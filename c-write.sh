#!/bin/bash


# usage help
usage() {
    echo "Usage: $0 <input_file> <station_type> <consumer_type> [plant_id]"
    echo "Options:"
    echo "  <input_file>: Path to the input CSV file"
    echo "  <station_type>: Type of station (hvb, hva, lv)"
    echo "  <consumer_type>: Consumer type (comp, indiv, all)"
    echo "  [plant_id]: (Optional) Filter by power plant ID"
    echo "  -h: Display help"
}

#  arguments validation
if [[ "$1" == "-h" || $# -lt 3 ]]; then
    usage
    exit 1
fi

INPUT_FILE=$1
STATION_TYPE=$2
CONSUMER_TYPE=$3
PLANT_ID=${4:-}

# Validate station type and consumer type
if [[ "$STATION_TYPE" != "hvb" && "$STATION_TYPE" != "hva" && "$STATION_TYPE" != "lv" ]]; then
    echo "Error: Invalid station type."
    usage
    exit 1
fi

if [[ "$CONSUMER_TYPE" != "comp" && "$CONSUMER_TYPE" != "indiv" && "$CONSUMER_TYPE" != "all" ]]; then
    echo "Error: Invalid consumer type."
    usage
    exit 1
fi

if [[ ("$STATION_TYPE" == "hvb" || "$STATION_TYPE" == "hva") && "$CONSUMER_TYPE" != "comp" ]]; then
    echo "Error: Invalid combination of station and consumer type."
    usage
    exit 1
fi

# Check input file existence
if [[ ! -f "$INPUT_FILE" ]]; then
    echo "Error: Input file not found."
    exit 1
fi

# Ensure tmp and graphs directories exist
mkdir -p tmp graphs
rm -rf tmp/*

# Compile the C program if not already compiled
if [[ ! -f codeC/cwire ]]; then
    echo "Compiling the C program..."
    pushd codeC > /dev/null
    make clean && make
    if [[ $? -ne 0 ]]; then
        echo "Error: Compilation failed."
        exit 1
    fi
    popd > /dev/null
fi

# Prepare the filtered file
FILTERED_FILE="tmp/filtered_data.csv"
awk -F';' -v station="$STATION_TYPE" -v consumer="$CONSUMER_TYPE" -v plant="$PLANT_ID" 'BEGIN {OFS=";"} {
    if ((station == "hvb" && $2 != "-" && consumer == "comp" && $5 != "-") ||
        (station == "hva" && $3 != "-" && consumer == "comp" && $5 != "-") ||
        (station == "lv" && $4 != "-")) {
        if (plant == "" || $1 == plant) {
            print $0
        }
    }
}' "$INPUT_FILE" > "$FILTERED_FILE"

if [[ ! -s "$FILTERED_FILE" ]]; then
    echo "Error: No matching data found."
    exit 1
fi

# Run the C program
OUTPUT_FILE="tmp/output_${STATION_TYPE}_${CONSUMER_TYPE}${PLANT_ID:+_$PLANT_ID}.csv"
echo "Running the C program..."
codeC/cwire "$FILTERED_FILE" "$OUTPUT_FILE"
if [[ $? -ne 0 ]]; then
    echo "Error: C program failed."
    exit 1
fi

# lV-specific processing for "all"
if [[ "$STATION_TYPE" == "lv" && "$CONSUMER_TYPE" == "all" ]]; then
    MINMAX_FILE="tmp/lv_all_minmax.csv"
    echo "Processing LV min/max..."
    sort -t':' -k3 -n "$OUTPUT_FILE" | head -n 10 > "$MINMAX_FILE"
    sort -t':' -k3 -n "$OUTPUT_FILE" | tail -n 10 >> "$MINMAX_FILE"
    echo "Min/max data saved to $MINMAX_FILE"
fi

# graph (bonus)
if [[ "$STATION_TYPE" == "lv" && "$CONSUMER_TYPE" == "all" ]]; then
    echo "Generating graph..."
    gnuplot -e "set terminal png; set output 'graphs/lv_all_minmax.png';
    set title 'LV Stations Min/Max Consumption';
    set xlabel 'Stations'; set ylabel 'Consumption (kWh)';
    plot '$MINMAX_FILE' using 2:3 with bars title 'Consumption';"
    echo "Graph saved to graphs/lv_all_minmax.png"
fi


echo "Processing complete. Results saved to $OUTPUT_FILE."
exit 0
