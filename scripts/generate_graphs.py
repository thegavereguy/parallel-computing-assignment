import pandas as pd
import plotly.express as px
import os
import glob

# Path to the folder containing results
results_folder = 'results'
files_pattern = os.path.join(results_folder, '*.txt')  # Matches all .txt files in the folder

# Create an empty DataFrame to hold all data
all_data = pd.DataFrame()

# Process each file
for file_path in glob.glob(files_pattern):
    # Extract the test name and thread number from the filename
    file_name = os.path.basename(file_path)
    test_name_parts = file_name.replace('.txt', '').split('.')  # Split at '.' to get parts
    test_name = test_name_parts[0]
    thread_count = test_name_parts[1] if len(test_name_parts) > 1 else 'Unknown'  # Use 'Unknown' if thread count is missing

    # Load data from the CSV file
    data = pd.read_csv(file_path)
    data['Test'] = test_name
    data['Threads'] = thread_count  # Add thread count as a column

    # Append to the main DataFrame
    all_data = pd.concat([all_data, data])

# Convert thread count to string for consistent labeling
all_data['Threads'] = all_data['Threads'].astype(str)

# Create the output folder for images
output_folder = 'results/output_images'
os.makedirs(output_folder, exist_ok=True)

# Generate and save an image plot for each unique test
for test_name, test_data in all_data.groupby('Test'):
    fig = px.line(
        test_data,
        x='DIMENSION',
        y='MEAN',
        error_y='SD',   # Adds error bars based on the standard deviation column
        color='Threads',  # Different colors for each thread count
        markers=True,
        title=f"Execution Times for {test_name} (Comparing Threads)",
        labels={
            "DIMENSION": "Matrix Dimension (N)",
            "MEAN": "Mean Execution Time (ms)",
            "Threads": "Thread Count",
            "SD": "Standard Deviation"
        }
    )

    fig.update_yaxes(type="log")  # Set y-axis to logarithmic scale
    fig.update_layout(legend_title="Thread Count")
    fig.update_traces(error_y=dict(color='rgba(255, 0, 0, 0.6)', thickness=0))  

    # Save each plot as an image
    output_path = os.path.join(output_folder, f'{test_name}.png')
    fig.write_image(output_path, format='png', width=1200, height=800)
    fig.show()
    print(f"Graph for {test_name} saved as: {output_path}")

