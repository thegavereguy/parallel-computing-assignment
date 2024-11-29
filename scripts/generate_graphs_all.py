import pandas as pd
import plotly.graph_objects as go
import os
import glob

# Paths to the folders containing results
benchmarks_folder = "results/benchmarks"
cache_folder = "results/cache"
benchmarks_pattern = os.path.join(
    benchmarks_folder, "*.csv"
)  # Matches all .txt files in benchmarks
cache_pattern = os.path.join(cache_folder, "*.csv")  # Matches all .csv files in cache

# Create empty DataFrames to hold benchmark and cache data
all_benchmarks = pd.DataFrame()
all_cache = pd.DataFrame()

# Load benchmark data
for file_path in glob.glob(benchmarks_pattern):
    print(f"Processing file: {file_path}")
    # Extract the test name and thread number from the filename
    file_name = os.path.basename(file_path)
    test_name_parts = file_name.replace(".csv", "").split(
        "."
    )  # Split at '.' to get parts

    optimization_level = test_name_parts[1]

    test_name = test_name_parts[0]
    thread_count = (
        test_name_parts[2] if len(test_name_parts) > 1 else "Unknown"
    )  # Use 'Unknown' if thread count is missing

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

# Load cache data
for file_path in glob.glob(cache_pattern):
    print(f"Processing file: {file_path}")
    # Extract the test name and thread number from the filename
    file_name = os.path.basename(file_path)
    test_name_parts = file_name.replace(".csv", "").split(
        "."
    )  # Split at '.' to get parts
    test_name = test_name_parts[0]
    optimization_level = test_name_parts[1]
    thread_count = (
        test_name_parts[2] if len(test_name_parts) > 1 else "Unknown"
    )  # Use 'Unknown' if thread count is missing

    # Load data from the CSV file
    data = pd.read_csv(file_path)
    data["Test"] = test_name
    data["Threads"] = thread_count  # Add thread count as a column

    data["CacheMissRatio"] = data["CACHEMISS"] / data["CACHEREF"]
    data["opt_level"] = optimization_level

    # Append to the main DataFrame
    all_cache = pd.concat([all_cache, data])

# Convert thread count to string for consistent labeling
all_benchmarks["Threads"] = all_benchmarks["Threads"].astype(str)
all_cache["Threads"] = all_cache["Threads"].astype(str)

# Create the output folder for images
output_folder = "results/output_images"
os.makedirs(output_folder, exist_ok=True)

# Generate and save an image plot for each unique test
for test_name in all_benchmarks["Test"].unique():
    # Filter data for the current test
    benchmark_data = all_benchmarks[all_benchmarks["Test"] == test_name]
    cache_data = all_cache[all_cache["Test"] == test_name]

    fig = go.Figure()

    # Plot each thread count as a separate line for benchmarks
    for (thread_count, opt_level), thread_data in benchmark_data.groupby(
        ["Threads", "opt_level"]
    ):
        fig.add_trace(
            go.Scatter(
                x=thread_data["DIMENSION"],
                y=thread_data["MEAN"],
                mode="lines+markers",
                name=f"Execution time: {thread_count} threads, opt_level: {opt_level}",
                error_y=dict(
                    type="data",
                    symmetric=False,
                    array=thread_data[
                        "ErrorPlus"
                    ],  # Upper error specific to this thread count
                    arrayminus=thread_data[
                        "ErrorMinus"
                    ],  # Lower error specific to this thread count
                    color="rgba(255, 0, 0, 0.6)",  # Customize error bar color
                    thickness=1.5,  # Customize error bar thickness
                ),
            )
        )

    # Grafico del rapporto cache misses/references
    if not cache_data.empty:
        for (thread_count, opt_level), thread_data in cache_data.groupby(
            ["Threads", "opt_level"]
        ):
            print(f"Cache data for {test_name} with {thread_count} threads:")
            print(thread_data)
            fig.add_trace(
                go.Scatter(
                    x=thread_data["DIMENSION"],
                    y=thread_data["CacheMissRatio"],
                    mode="lines+markers",
                    name=f"{test_name} Cache Miss Ratio: {thread_count} threads, opt_level: {opt_level}",
                    yaxis="y2",
                    line=dict(dash="dot"),  # Linea tratteggiata
                    hovertemplate="DIMENSION: %{x}<br>Cache Miss Ratio: %{y:.2%}<br>Threads: %{name}<extra></extra>",
                )
            )

    # Update layout to include secondary y-axis

    # Update layout to include secondary y-axis for cache metrics
    fig.update_layout(
        title=f"Execution Times and Cache Metrics for {test_name}",
        xaxis_title="Matrix Dimension (N)",
        yaxis_title="Mean Execution Time (ms)",
        yaxis_type="log",  # Set y-axis for execution time to logarithmic scale
        xaxis_type="log",  # Set y-axis for execution time to logarithmic scale
        # xaxis_type="log",  # Set y-axis for execution time to logarithmic scale
        yaxis=dict(
            title="Mean Execution Time (ms)",
            type="log",  # Primary y-axis is logarithmic
        ),
        yaxis2=dict(
            title="Cache Metrics ()",
            overlaying="y",  # Overlay on the same graph
            side="right",  # Place on the right
            type="linear",  # Linear scale for the secondary axis
        ),
        legend_title="Metric/Thread Count",
    )

    # Save each plot as an image
    output_path = os.path.join(output_folder, f"{test_name}.png")
    fig.write_image(output_path, format="png", width=1600, height=1000)
    fig.show()
    print(f"Graph for {test_name} saved as: {output_path}")
