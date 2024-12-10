import pandas as pd
import plotly.graph_objects as go
import os
import glob


benchmarks_folder = "results/benchmarks"
cache_folder = "results/cache"
benchmarks_pattern = os.path.join(benchmarks_folder, "*.csv")
cache_pattern = os.path.join(cache_folder, "*.csv")


all_benchmarks = pd.DataFrame()
all_cache = pd.DataFrame()


for file_path in glob.glob(benchmarks_pattern):
    print(f"Processing file: {file_path}")

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


for file_path in glob.glob(cache_pattern):
    print(f"Processing file: {file_path}")

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


all_benchmarks["Threads"] = all_benchmarks["Threads"].astype(str)
all_cache["Threads"] = all_cache["Threads"].astype(str)


output_folder = "results/output_images"
os.makedirs(output_folder, exist_ok=True)


for test_name in all_benchmarks["Test"].unique():

    benchmark_data = all_benchmarks[all_benchmarks["Test"] == test_name]
    cache_data = all_cache[all_cache["Test"] == test_name]

    fig = go.Figure()

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
                    color="rgba(255, 0, 0, 0.6)",
                    thickness=1.5,
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
                    line=dict(dash="dot"),
                    hovertemplate="DIMENSION: %{x}<br>Cache Miss Ratio: %{y:.2%}<br>Threads: %{name}<extra></extra>",
                )
            )

    fig.update_layout(
        title=f"Execution Times and Cache Metrics for {test_name}",
        xaxis_title="Matrix Dimension (N)",
        yaxis_title="Mean Execution Time (ms)",
        yaxis_type="log",
        xaxis_type="log",
        yaxis=dict(
            title="Mean Execution Time (ms)",
            type="log",
        ),
        yaxis2=dict(
            title="Cache Metrics ()",
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
        ),
    )

    output_path = os.path.join(output_folder, f"{test_name}.png")
    fig.write_image(output_path, format="png", width=1600, height=1000)
    fig.show()
    print(f"Graph for {test_name} saved as: {output_path}")
