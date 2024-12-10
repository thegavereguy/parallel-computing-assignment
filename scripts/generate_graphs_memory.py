import pandas as pd
import plotly.graph_objects as go
import os
import tkinter as tk
from tkinter import filedialog


def select_files(title, filetypes):
    root = tk.Tk()
    root.withdraw()
    file_paths = filedialog.askopenfilenames(title=title, filetypes=filetypes)
    return file_paths


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


all_benchmarks = pd.DataFrame()


benchmark_files = select_files("Select Benchmark CSV Files", [("CSV Files", "*.csv")])
if benchmark_files:
    for file_path in benchmark_files:
        print(f"Processing benchmark file: {file_path}")

        file_name = os.path.basename(file_path)
        test_name_parts = file_name.replace(".csv", "").split(".")
        optimization_level = test_name_parts[1]
        test_name = test_name_parts[0]
        thread_count = test_name_parts[2] if len(test_name_parts) > 1 else "Unknown"

        data = pd.read_csv(file_path)
        data["Test"] = test_name
        data["Threads"] = thread_count
        data["opt_level"] = optimization_level

        element_size = 8
        data["BytesTransferred"] = data["DIMENSION"] ** 2 * element_size * 2

        data["Bandwidth"] = data["BytesTransferred"] / (data["MEAN"] / 1000)

        all_benchmarks = pd.concat([all_benchmarks, data])

else:
    print("No benchmark files selected. Exiting.")
    exit()


all_benchmarks["Threads"] = all_benchmarks["Threads"].astype(str)


output_folder = "results/output_images"
os.makedirs(output_folder, exist_ok=True)


fig = go.Figure()


for (test_name, thread_count, opt_level), thread_data in all_benchmarks.groupby(
    ["Test", "Threads", "opt_level"]
):
    fig.add_trace(
        go.Scatter(
            x=thread_data["DIMENSION"],
            y=thread_data["Bandwidth"] / (1024**3),
            mode="lines+markers",
            name=f"{test_name} ({thread_count} threads, opt_level: {opt_level})",
            hovertemplate=(
                "Matrix Dimension: %{x}<br>Bandwidth: %{y:.2f} GB/s<br>Threads: %{name}<extra></extra>"
            ),
        )
    )


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


fig.update_layout(
    title="Memory Bandwidth vs Matrix Dimension",
    xaxis_title="Matrix Dimension (N)",
    yaxis_title="Memory Bandwidth (GB/s)",
    xaxis_type="log",
    yaxis_type="linear",
    legend_title="Test/Thread Count",
    legend=dict(
        orientation="h",
        yanchor="top",
        y=-0.2,
        xanchor="center",
        x=0.5,
    ),
)


output_path = os.path.join(output_folder, "memory_bandwidth_results_with_max.png")
fig.write_image(output_path, format="png", width=1600, height=1000)


fig.show()

print("Graph saved as memory_bandwidth_results_with_max.png and displayed.")
