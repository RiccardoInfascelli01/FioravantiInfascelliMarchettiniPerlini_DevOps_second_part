#!/bin/bash
#SBATCH --job-name=grayscale_conversion
#SBATCH --output=result.out
#SBATCH --error=result.err
#SBATCH --time=00:10:00
#SBATCH --partition=g100_usr_dbg
#SBATCH --ntasks=1

module load singularity

# 1. Pulisci la cartella random_images
rm -rf random_images
mkdir -p random_images

# 2. Genera immagini casuali
NUM_IMAGES=10
for i in $(seq 0 $((NUM_IMAGES - 1))); do
    python3 random_gen_images.py "$i"
done

# 3. Crea cartella di output per immagini convertite
OUTPUT_DIR="grayscale_output"
mkdir -p "$OUTPUT_DIR"

# 4. Esegui il programma di conversione sulle immagini generate
singularity run image_converter.sif random_images "$OUTPUT_DIR" Average
