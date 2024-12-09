import pandas as pd
import plotly.graph_objects as go
import os
import tkinter as tk
from tkinter import filedialog


# Function to select files using a file dialog
def select_files(title, filetypes):
    root = tk.Tk()
    root.withdraw()  # Do not show the root window
    file_paths = filedialog.askopenfilenames(title=title, filetypes=filetypes)
    return file_paths


# Prompt user to input system memory bandwidth (GB/s)
try:
    max_bandwidth_gb_s = float(
        input(
            "Enter the maximum theoretical memory bandwidth of your system (in GB/s): "
        )
    )
except ValueError:
    print(
        "Invalid input. Please enter a valid numeric value for the maximum bandwidth."
    )
    exit()

# Create empty DataFrame to hold benchmark data
all_benchmarks = pd.DataFrame()

# Select benchmark CSV files
benchmark_files = select_files("Select Benchmark CSV Files", [("CSV Files", "*.csv")])
if benchmark_files:  # Only process if files are selected
    for file_path in benchmark_files:
        print(f"Processing benchmark file: {file_path}")
        # Extract test name and thread number from filename
        file_name = os.path.basename(file_path)
        test_name_parts = file_name.replace(".csv", "").split(".")
        optimization_level = test_name_parts[1]
        test_name = test_name_parts[0]
        thread_count = test_name_parts[2] if len(test_name_parts) > 1 else "Unknown"

        # Load data from the CSV file
        data = pd.read_csv(file_path)
        data["Test"] = test_name
        data["Threads"] = thread_count  # Add thread count as a column
        data["opt_level"] = optimization_level

        # Calculate bytes transferred (assuming double precision, 8 bytes per element)
        element_size = 8  # Double precision (8 bytes per element)
        data["BytesTransferred"] = (
            data["DIMENSION"] ** 2 * element_size * 2
        )  # Matrix size * 2 (read + write)

        # Calculate memory bandwidth (Bytes per second)
        data["Bandwidth"] = data["BytesTransferred"] / (
            data["MEAN"] / 1000
        )  # Convert time from ms to s

        # Append to the main DataFrame
        all_benchmarks = pd.concat([all_benchmarks, data])

else:
    print("No benchmark files selected. Exiting.")
    exit()

# Convert thread count to string for consistent labeling
all_benchmarks["Threads"] = all_benchmarks["Threads"].astype(str)

# Create the output folder for images
output_folder = "results/output_images"
os.makedirs(output_folder, exist_ok=True)

# Create one plot for all the data
fig = go.Figure()

# Plot memory bandwidth against matrix dimension
for (test_name, thread_count, opt_level), thread_data in all_benchmarks.groupby(
    ["Test", "Threads", "opt_level"]
):
    fig.add_trace(
        go.Scatter(
            x=thread_data["DIMENSION"],
            y=thread_data["Bandwidth"] / (1024**3),  # Convert to GB/s
            mode="lines+markers",
            name=f"{test_name} ({thread_count} threads, opt_level: {opt_level})",
            hovertemplate=(
                "Matrix Dimension: %{x}<br>Bandwidth: %{y:.2f} GB/s<br>Threads: %{name}<extra></extra>"
            ),
        )
    )

# Add the maximum theoretical bandwidth as a horizontal line
fig.add_trace(
    go.Scatter(
        x=all_benchmarks["DIMENSION"].unique(),
        y=[max_bandwidth_gb_s] * len(all_benchmarks["DIMENSION"].unique()),
        mode="lines",
        line=dict(color="red", dash="dash"),
        name="Theoretical Max Bandwidth",
        hovertemplate="Theoretical Max Bandwidth: %{y:.2f} GB/s<extra></extra>",
    )
)

# Update layout
fig.update_layout(
    title="Memory Bandwidth vs Matrix Dimension",
    xaxis_title="Matrix Dimension (N)",
    yaxis_title="Memory Bandwidth (GB/s)",
    xaxis_type="log",  # Logarithmic scale for x-axis (dimensions)
    yaxis_type="linear",  # Linear scale for bandwidth
    legend_title="Test/Thread Count",
    legend=dict(
        orientation="h",  # Horizontal legend
        yanchor="top",  # Align the top of the legend
        y=-0.2,  # Position the legend below the graph
        xanchor="center",  # Center the legend horizontally
        x=0.5,  # Center the legend on the x-axis
    ),
)

# Save the final plot as an image
output_path = os.path.join(output_folder, "memory_bandwidth_results_with_max.png")
fig.write_image(output_path, format="png", width=1600, height=1000)

# Show the graph
fig.show()

print("Graph saved as memory_bandwidth_results_with_max.png and displayed.")
