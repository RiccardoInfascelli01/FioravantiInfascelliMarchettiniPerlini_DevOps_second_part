#!/bin/bash
#SBATCH --job-name=grayscale_conversion
#SBATCH --output=result.out
#SBATCH --error=result.err
#SBATCH --time=00:10:00
#SBATCH --partition=debug
#SBATCH --ntasks=1

module load singularity

INPUT_FILE=input.ppm
OUTPUT_FILE=output.pgm

singularity run image_converter.sif "$INPUT_FILE" "$OUTPUT_FILE"
