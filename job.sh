#!/bin/bash
#SBATCH --job-name=grayscale_conversion
#SBATCH --output=result.out
#SBATCH --error=result.err
#SBATCH --time=00:10:00
#SBATCH --partition=g100_usr_dbg
#SBATCH --ntasks=1

# 1. Carica Singularity
module load singularity

# 2. Genera immagini casuali
NUM_IMAGES=10
for i in $(seq 0 $((NUM_IMAGES - 1))); do
    python3 generate_random_image.py "$i"
done

# 3. Crea cartella di output
OUTPUT_DIR="grayscale_output"
mkdir -p "$OUTPUT_DIR"

# 4. Esegui il programma (input: random_images/, output: grayscale_output/)
singularity run image_converter.sif random_images "$OUTPUT_DIR" Average
