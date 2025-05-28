#!/bin/bash
#SBATCH --job-name=grayscale_conversion
#SBATCH --output=result.out
#SBATCH --error=result.err
#SBATCH --time=00:10:00
#SBATCH --partition=debug
#SBATCH --ntasks=1

module load singularity

# Supponendo che l'app sia configurata per accettare input/output come argomenti
singularity run image_converter.sif input.ppm output.pgm
