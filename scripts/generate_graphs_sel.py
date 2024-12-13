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


all_benchmarks = pd.DataFrame()
all_cache = pd.DataFrame()

try:
    chart_name = input("Enter the name of the chart: ")
except ValueError:
    print("Invalid input. Please enter a valid name for the chart.")
    exit()


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

        data["ErrorMinus"] = data["MEAN"] - data["MINT"]
        data["ErrorPlus"] = data["MAXT"] - data["MEAN"]
        data["opt_level"] = optimization_level

        all_benchmarks = pd.concat([all_benchmarks, data])

else:
    print("No benchmark files selected. Skipping benchmark data processing.")


cache_files = select_files("Select Cache CSV Files", [("CSV Files", "*.csv")])
if cache_files:

    for file_path in cache_files:
        print(f"Processing cache file: {file_path}")

        file_name = os.path.basename(file_path)
        test_name_parts = file_name.replace(".csv", "").split(".")
        test_name = test_name_parts[0]
        optimization_level = test_name_parts[1]
        thread_count = test_name_parts[2] if len(test_name_parts) > 1 else "Unknown"

        data = pd.read_csv(file_path)
        data["Test"] = test_name
        data["Threads"] = thread_count

        data["CacheMissRatio"] = data["CACHEMISS"] / data["CACHEREF"]
        data["opt_level"] = optimization_level

        all_cache = pd.concat([all_cache, data])

else:
    print("No cache files selected. Skipping cache data processing.")


if all_benchmarks.empty and all_cache.empty:
    print("No data to process. Exiting.")
else:
    if not all_benchmarks.empty:
        all_benchmarks["Threads"] = all_benchmarks["Threads"].astype(str)

    if not all_cache.empty:
        all_cache["Threads"] = all_cache["Threads"].astype(str)

    output_folder = "results/output_images"
    os.makedirs(output_folder, exist_ok=True)

    fig = go.Figure()

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

    fig.update_layout(
        title=chart_name,
        xaxis_title="Matrix Dimension (N)",
        yaxis_title="Mean Execution Time (ms)",
        yaxis_type="log",
        xaxis_type="log",
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
            orientation="h",
            yanchor="top",
            y=-0.2,
            xanchor="center",
            x=0.5,
            font=dict(size=15),
        ),
    )

    output_path = os.path.join(output_folder, "combined_results.png")
    fig.write_image(output_path, format="png", width=1600, height=1000)

    fig.show()

    print("Graph saved as combined_results.png and displayed.")
