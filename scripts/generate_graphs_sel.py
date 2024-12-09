import pandas as pd
import plotly.graph_objects as go
import os
import tkinter as tk
from tkinter import filedialog


# Function to select files using a file dialog
def select_files(title, filetypes):
    # Hide the main tkinter window
    root = tk.Tk()
    root.withdraw()  # Do not show the root window
    file_paths = filedialog.askopenfilenames(title=title, filetypes=filetypes)
    return file_paths


# Create empty DataFrames to hold benchmark and cache data
all_benchmarks = pd.DataFrame()
all_cache = pd.DataFrame()

# Select benchmark CSV files
benchmark_files = select_files("Select Benchmark CSV Files", [("CSV Files", "*.csv")])
if benchmark_files:  # Only process if files are selected
    # Load benchmark data
    for file_path in benchmark_files:
        print(f"Processing benchmark file: {file_path}")
        # Extract the test name and thread number from the filename
        file_name = os.path.basename(file_path)
        test_name_parts = file_name.replace(".csv", "").split(".")
        optimization_level = test_name_parts[1]
        test_name = test_name_parts[0]
        thread_count = test_name_parts[2] if len(test_name_parts) > 1 else "Unknown"

        # Load data from the CSV file
        data = pd.read_csv(file_path)
        data["Test"] = test_name
        data["Threads"] = thread_count  # Add thread count as a column

        # Calculate asymmetric error bars
        data["ErrorMinus"] = data["MEAN"] - data["MINT"]  # Error on the lower side
        data["ErrorPlus"] = data["MAXT"] - data["MEAN"]  # Error on the upper side
        data["opt_level"] = optimization_level

        # Append to the main DataFrame
        all_benchmarks = pd.concat([all_benchmarks, data])

else:
    print("No benchmark files selected. Skipping benchmark data processing.")

# Select cache CSV files
cache_files = select_files("Select Cache CSV Files", [("CSV Files", "*.csv")])
if cache_files:  # Only process if files are selected
    # Load cache data
    for file_path in cache_files:
        print(f"Processing cache file: {file_path}")
        # Extract the test name and thread number from the filename
        file_name = os.path.basename(file_path)
        test_name_parts = file_name.replace(".csv", "").split(".")
        test_name = test_name_parts[0]
        optimization_level = test_name_parts[1]
        thread_count = test_name_parts[2] if len(test_name_parts) > 1 else "Unknown"

        # Load data from the CSV file
        data = pd.read_csv(file_path)
        data["Test"] = test_name
        data["Threads"] = thread_count  # Add thread count as a column

        data["CacheMissRatio"] = data["CACHEMISS"] / data["CACHEREF"]
        data["opt_level"] = optimization_level

        # Append to the main DataFrame
        all_cache = pd.concat([all_cache, data])

else:
    print("No cache files selected. Skipping cache data processing.")

# Proceed to create plots only if data exists
if all_benchmarks.empty and all_cache.empty:
    print("No data to process. Exiting.")
else:
    if not all_benchmarks.empty:
        all_benchmarks["Threads"] = all_benchmarks["Threads"].astype(str)

    if not all_cache.empty:
        all_cache["Threads"] = all_cache["Threads"].astype(str)

    # Create the output folder for images
    output_folder = "results/output_images"
    os.makedirs(output_folder, exist_ok=True)

    # Create one plot for all the data
    fig = go.Figure()

    # Plot execution times (benchmark data)
    if not all_benchmarks.empty:
        for (test_name, thread_count, opt_level), thread_data in all_benchmarks.groupby(
            ["Test", "Threads", "opt_level"]
        ):
            fig.add_trace(
                go.Scatter(
                    x=thread_data["DIMENSION"],
                    y=thread_data["MEAN"],
                    mode="lines+markers",
                    name=f"Execution time: {test_name} ({thread_count} threads, opt_level: {opt_level})",
                    error_y=dict(
                        type="data",
                        symmetric=False,
                        array=thread_data["ErrorPlus"],
                        arrayminus=thread_data["ErrorMinus"],
                        color="rgba(255, 0, 0, 0.6)",
                        thickness=1.5,
                    ),
                )
            )

    # Plot cache miss ratio (cache data)
    if not all_cache.empty:
        for (test_name, thread_count, opt_level), thread_data in all_cache.groupby(
            ["Test", "Threads", "opt_level"]
        ):
            fig.add_trace(
                go.Scatter(
                    x=thread_data["DIMENSION"],
                    y=thread_data["CacheMissRatio"],
                    mode="lines+markers",
                    name=f"Cache Miss Ratio: {test_name} ({thread_count} threads, opt_level: {opt_level})",
                    yaxis="y2",
                    line=dict(dash="dot"),
                    hovertemplate="DIMENSION: %{x}<br>Cache Miss Ratio: %{y:.2%}<br>Threads: %{name}<extra></extra>",
                )
            )

    # Update layout to include secondary y-axis for cache metrics
    fig.update_layout(
        title="Execution Times and Cache Metrics",
        xaxis_title="Matrix Dimension (N)",
        yaxis_title="Mean Execution Time (ms)",
        yaxis_type="log",  # Logarithmic scale for execution time
        xaxis_type="log",  # Logarithmic scale for x-axis (dimensions)
        yaxis=dict(
            title="Mean Execution Time (ms)",
            type="log",
        ),
        yaxis2=dict(
            title="Cache Metrics (%)",
            overlaying="y",
            side="right",
            type="linear",
        ),
        legend_title="Metric/Thread Count",
        legend=dict(
            orientation="h",  # Horizontal legend
            yanchor="top",  # Align the top of the legend
            y=-0.2,  # Position the legend below the graph
            xanchor="center",  # Center the legend horizontally
            x=0.5,  # Center the legend on the x-axis
        ),
    )

    # Save the final plot as an image
    output_path = os.path.join(output_folder, "combined_results.png")
    fig.write_image(output_path, format="png", width=1600, height=1000)

    # Show the graph
    fig.show()

    print("Graph saved as combined_results.png and displayed.")
