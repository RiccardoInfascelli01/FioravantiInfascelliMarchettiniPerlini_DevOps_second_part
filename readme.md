## Authors

- Michele Fioravanti (11098551)  
- Riccardo Infascelli (10742969)  
- Bianca Maria Marchettini (11098150)  
- Aurora Perlini (10788118)  


# Grayscale Image Conversion Utility

This project provides a utility to convert `.ppm` images to `.pgm` format using various grayscale conversion methods. The tool processes all `.ppm` images in a specified input folder and saves the converted `.pgm` images in an output folder.

---

## Features

- **Supported Grayscale Methods**:
  - `Lightness`: Average of the brightest and darkest color channels.
  - `Average`: Mean of all three color channels.
  - `Luminosity`: Weighted average emphasizing green, followed by red, and then blue.
  - `Desaturation`: Average of the maximum and minimum color channel values.
  - `RedChannel`: Uses the red channel as the grayscale value.
  - `GreenChannel`: Uses the green channel as the grayscale value.
  - `BlueChannel`: Uses the blue channel as the grayscale value.

- **Batch Processing**:
  - Converts all `.ppm` images in the input folder.
  - Saves the converted `.pgm` images in the output folder.

- **Some Error Handling stuff (already handled for you):**:
  - Validates input arguments.
  - Handles invalid grayscale methods.
  - Skips files that cannot be read or written.

---

## Usage

### Command-Line Syntax

```bash
./convert_grayscale <input_folder> <output_folder> <grayscale_method>
```

## Part 2: Automation, Containerization & Cluster Execution

After implementing the test suites in the first part of the project ([link to part 1 repository](https://github.com/marchettinibiancamaria/FioravantiInfascelliMarchettiniPerlini_DevOps_first_part.git)), we focused on fully automating the grayscale image conversion pipeline. This included building a container, compiling and testing the project on every push, and running the job on the **CINECA G100 cluster** using **GitHub Actions** and **SLURM** for job scheduling.

### Workflow Automation

We defined a GitHub Actions workflow that performs the following tasks automatically on each push:

1. **Builds the project** and compiles all necessary binaries.
2. **Runs all test suites** to ensure the correctness of the implementation.
3. **Builds a Singularity container** with all required dependencies.
4. **Deploys the container** to the CINECA G100 cluster.
5. **Submits the job to SLURM**, the workload manager used by CINECA, to execute the grayscale conversion on the cluster.

### Containerization with Singularity

The container is defined in the `Singularity.def` file and includes everything needed to run the conversion program in an isolated, reproducible environment. This ensures compatibility across different systems and makes it easier to run on HPC environments like CINECA.

### Secure Cluster Authentication

To securely authenticate with the CINECA cluster, we used **GitHub Secrets**. These secrets store the **private and public keys** required to access the cluster. Since CINECA uses short-lived certificates (valid for 12 hours), we updated the credentials regularly to maintain secure access throughout development and testing.

### Job Submission with SLURM

Cluster jobs are submitted using the `job.sh` script. This script is configured for SLURM, the resource manager used by CINECA, which handles the scheduling and allocation of compute resources. SLURM allows us to define:

- Number of nodes and tasks
- Execution time limits
- Output file redirection
- Resource usage monitoring

This setup ensures that our image conversion jobs are properly queued, executed, and logged within the cluster environment.

### Example Image Processing

We used the `random_gen_images.py` script to generate random `.ppm` images to test the grayscale conversion functionality at scale. These were processed on the cluster using the **Average** grayscale method.

Converted `.pgm` outputs are available in the `results` directory and demonstrate the system working end-to-end on the cluster.

### Collaboration & Troubleshooting

The entire team collaborated on identifying potential challenges and edge cases in automation, testing, and cluster execution. Each member contributed to both planning and implementation, ensuring robust coverage and smooth deployment.

## License
This project is licensed under the [MIT License](./LICENSE).
