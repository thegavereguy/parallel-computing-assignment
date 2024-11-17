import pandas as pd
import plotly.graph_objects as go
import os
import glob

# Path to the folder containing results
results_folder = "results/benchmarks"
files_pattern = os.path.join(
    results_folder, "*.csv"
)  # Matches all .csv files in the folder

# Create an empty DataFrame to hold all data
all_data = pd.DataFrame()

# Process each file
for file_path in glob.glob(files_pattern):
    print(f"Processing file: {file_path}")
    # Extract the test name and thread number from the filename
    file_name = os.path.basename(file_path)
    test_name_parts = file_name.replace(".csv", "").split(
        "."
    )  # Split at '.' to get parts
    test_name = test_name_parts[0]
    thread_count = (
        test_name_parts[1] if len(test_name_parts) > 1 else "Unknown"
    )  # Use 'Unknown' if thread count is missing

    # Load data from the CSV file
    data = pd.read_csv(file_path)
    data["Test"] = test_name
    data["Threads"] = thread_count  # Add thread count as a column

    # Calculate asymmetric error bars
    data["ErrorMinus"] = data["MEAN"] - data["MINT"]  # Error on the lower side
    data["ErrorPlus"] = data["MAXT"] - data["MEAN"]  # Error on the upper side

    # Append to the main DataFrame
    all_data = pd.concat([all_data, data])

# Convert thread count to string for consistent labeling
all_data["Threads"] = all_data["Threads"].astype(str)

# Create the output folder for images
output_folder = "results/output_images"
os.makedirs(output_folder, exist_ok=True)

# Generate and save an image plot for each unique test
for test_name, test_data in all_data.groupby("Test"):
    fig = go.Figure()

    # Plot each thread count as a separate trace
    for thread_count, thread_data in test_data.groupby("Threads"):
        fig.add_trace(
            go.Scatter(
                x=thread_data["DIMENSION"],
                y=thread_data["MEAN"],
                mode="lines+markers",
                name=f"{thread_count}",
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

    fig.update_layout(
        title=f"Execution Times for {test_name} (Comparing Threads)",
        xaxis_title="Matrix Dimension (N)",
        yaxis_title="Mean Execution Time (ms)",
        yaxis_type="log",
        xaxis_type="log",
        legend_title="Thread Count",
    )

    # Save each plot as an image
    output_path = os.path.join(output_folder, f"{test_name}.png")
    fig.write_image(output_path, format="png", width=1200, height=800)
    fig.show()
    print(f"Graph for {test_name} saved as: {output_path}")
