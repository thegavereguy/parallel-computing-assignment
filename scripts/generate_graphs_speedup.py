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


def calculate_speedup(data, baseline_opt):
    baseline_data = data[data["opt_level"] == baseline_opt]
    speedup = []
    for _, row in data.iterrows():
        baseline_time = baseline_data[
            (baseline_data["DIMENSION"] == row["DIMENSION"])
            & (baseline_data["Threads"] == row["Threads"])
        ]["MEAN"]
        if not baseline_time.empty:
            speedup.append(baseline_time.values[0] / row["MEAN"])
        else:
            speedup.append(None)
    data["Speedup"] = speedup
    return data


try:
    chart_name = input("Enter the name of the chart: ")
except ValueError:
    print("Invalid input. Please enter a valid name for the chart.")
    exit()


benchmark_files = select_files("Select Benchmark CSV Files", [("CSV Files", "*.csv")])
if not benchmark_files:
    print("No benchmark files selected. Exiting.")
    exit()

all_benchmarks = pd.DataFrame()
for file_path in benchmark_files:
    print(f"Processing benchmark file: {file_path}")
    file_name = os.path.basename(file_path)
    parts = file_name.replace(".csv", "").split(".")
    test_name = parts[0]
    opt_level = parts[1] if len(parts) > 1 else "Unknown"
    thread_count = parts[2] if len(parts) > 2 else "Unknown"

    data = pd.read_csv(file_path)
    data["Test"] = test_name
    data["opt_level"] = opt_level
    data["Threads"] = thread_count

    all_benchmarks = pd.concat([all_benchmarks, data])

baseline_opt = input("Enter the optimization level to use as baseline: ")

if baseline_opt not in all_benchmarks["opt_level"].unique():
    print(
        f"Baseline optimization level '{baseline_opt}' not found in the data. Exiting."
    )
    exit()

all_benchmarks = calculate_speedup(all_benchmarks, baseline_opt)

all_benchmarks["Threads"] = all_benchmarks["Threads"].astype(str)

output_folder = "results/output_images"
os.makedirs(output_folder, exist_ok=True)

fig = go.Figure()

for (test_name, thread_count, opt_level), group_data in all_benchmarks.groupby(
    ["Test", "Threads", "opt_level"]
):
    fig.add_trace(
        go.Scatter(
            x=group_data["DIMENSION"],
            y=group_data["Speedup"],
            mode="lines+markers",
            name=f"{test_name} ({thread_count} Threads, OPT Level:{opt_level})",
        )
    )

fig.update_layout(
    title=dict(text=chart_name, font=dict(size=20)),
    xaxis_title=dict(text="Matrix Dimension (N)", font=dict(size=16)),
    yaxis_title=dict(text="Speedup", font=dict(size=16)),
    xaxis=dict(type="log"),
    legend=dict(
        orientation="h",
        yanchor="top",
        y=-0.2,
        xanchor="center",
        x=0.5,
        font=dict(size=16),
    ),
)

output_path = os.path.join(output_folder, "speedup_plot.png")
fig.write_image(output_path, format="png", width=1600, height=1000)
fig.show()

print(f"Speedup plot saved at {output_path}")
