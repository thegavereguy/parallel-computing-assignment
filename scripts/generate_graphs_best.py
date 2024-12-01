import pandas as pd
import plotly.graph_objects as go
from tkinter import Tk, filedialog


# Finestra di dialogo per selezionare i file
def select_files(title, filetypes):
    Tk().withdraw()  # Nasconde la finestra principale di Tkinter
    file_paths = filedialog.askopenfilenames(title=title, filetypes=filetypes)
    return file_paths


# Leggi i file CSV selezionati
def load_benchmark_data(files):
    all_data = pd.DataFrame()
    for file_path in files:
        # Nome del test e numero di thread dal nome file
        file_name = file_path.split("/")[-1]
        test_name, thread_count = file_name.replace(".csv", "").split(".")
        thread_count = int(thread_count)

        # Carica i dati CSV
        data = pd.read_csv(file_path)
        data["Test"] = test_name
        data["Threads"] = thread_count
        all_data = pd.concat([all_data, data])

    return all_data


# Carica i dati della cache e calcola la percentuale di cache misses
def load_cache_data(files):
    all_data = pd.DataFrame()
    for file_path in files:
        # Nome del test dal nome file
        file_name = file_path.split("/")[-1]
        test_name, thread_count = file_name.replace(".csv", "").split(".")
        thread_count = int(thread_count)

        # Carica i dati CSV
        data = pd.read_csv(file_path)
        data["Test"] = test_name
        data["Threads"] = thread_count

        # Calcola la percentuale di cache misses
        data["CacheMissRate (%)"] = (data["CACHEMISS"] / data["CACHEREF"]) * 100
        all_data = pd.concat([all_data, data])

    return all_data


# Funzione principale
def main():
    # Seleziona i file benchmark
    print("Seleziona i file CSV dei benchmark (tempi di trasposizione)...")
    benchmark_files = select_files(
        "Seleziona i file benchmark", [("File CSV", "*.csv"), ("Tutti i file", "*.*")]
    )
    if not benchmark_files:
        print("Nessun file benchmark selezionato. Uscita.")
        return

    # Seleziona i file cache
    print("Seleziona i file CSV della cache...")
    cache_files = select_files(
        "Seleziona i file cache", [("File CSV", "*.csv"), ("Tutti i file", "*.*")]
    )
    if not cache_files:
        print("Nessun file cache selezionato. Uscita.")
        return

    # Carica i dati
    benchmark_data = load_benchmark_data(benchmark_files)
    cache_data = load_cache_data(cache_files)

    # Crea il grafico
    fig = go.Figure()

    # Grafico dei tempi di esecuzione con bande di min/max
    for (test_name, thread_count), group_data in benchmark_data.groupby(
        ["Test", "Threads"]
    ):
        fig.add_trace(
            go.Scatter(
                x=group_data["DIMENSION"],
                y=group_data["MEAN"],
                mode="lines+markers",
                name=f"{test_name} (Threads: {thread_count})",
                hovertemplate="DIMENSION: %{x}<br>MEAN: %{y:.2f} ms<br>Threads: %{name}<extra></extra>",
            )
        )

        # Banda tra min e max
        fig.add_trace(
            go.Scatter(
                x=pd.concat([group_data["DIMENSION"], group_data["DIMENSION"][::-1]]),
                y=pd.concat([group_data["MINT"], group_data["MAXT"][::-1]]),
                fill="toself",
                fillcolor="rgba(0,100,200,0.2)",
                line=dict(color="rgba(255,255,255,0)"),
                hoverinfo="skip",
                showlegend=False,
            )
        )

    # Grafico della percentuale di cache misses
    for (test_name, thread_count), group_data in cache_data.groupby(
        ["Test", "Threads"]
    ):
        fig.add_trace(
            go.Scatter(
                x=group_data["DIMENSION"],
                y=group_data["CacheMissRate (%)"],
                mode="lines+markers",
                name=f"{test_name} Cache Miss Rate (Threads: {thread_count})",
                yaxis="y2",
                hovertemplate="DIMENSION: %{x}<br>Cache Miss Rate: %{y:.2f}%<br>Threads: %{name}<extra></extra>",
            )
        )

    # Configura il layout
    fig.update_layout(
        title="Tempi di esecuzione e Cache Miss Rate con Bande Min/Max",
        xaxis_title="Dimensione Matrice (N)",
        yaxis=dict(title="Tempo medio di esecuzione (ms)", type="log"),
        yaxis2=dict(title="Cache Miss Rate (%)", overlaying="y", side="right"),
        legend_title="Test/Threads",
    )

    # Mostra il grafico
    fig.show()


# Esegui il programma
if __name__ == "__main__":
    main()
