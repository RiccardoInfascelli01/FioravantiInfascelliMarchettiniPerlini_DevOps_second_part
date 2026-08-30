# Grayscale Image Processing — Testing & DevOps Project

Team project developed by a group of 4 students for the **Software Engineering for HPC** course at Politecnico di Milano.

This project provides a utility to convert `.ppm` images to `.pgm` format using different grayscale conversion methods.

The project was developed in two stages: the first focused on software testing and validation of a faulty implementation, while the second focused on automation, containerization, and execution on an HPC cluster.

## Authors

- Michele Fioravanti
- Riccardo Infascelli
- Bianca Maria Marchettini
- Aurora Perlini

## Features

Supported grayscale methods include:

- `Lightness`
- `Average`
- `Luminosity`
- `Desaturation`
- `RedChannel`
- `GreenChannel`
- `BlueChannel`

The utility supports batch processing of `.ppm` images and stores the converted `.pgm` files in a specified output directory.

## Usage

```bash
./convert_grayscale <input_folder> <output_folder> <grayscale_method>
```

## Part 1: Software Testing

The first part of the project focused on designing a test suite for an intentionally faulty implementation of the grayscale conversion utility.

The testing strategy included:

- **Bit-level tests** to verify the correctness of individual grayscale conversion methods.
- **Input and output limit tests** to detect missing range checks.
- **Pixel position mapping tests** to verify correct placement of converted pixels.
- **Metamorphic testing** to validate properties of the conversion algorithms even when an explicit expected output was not available.

The tests exposed errors both in grayscale-value computation and pixel mapping.

The same test suite was later applied to the corrected implementation used in Part 2, where the tests produced the expected results.

The original repository for Part 1 is available [here](https://github.com/marchettinibiancamaria/FioravantiInfascelliMarchettiniPerlini_DevOps_first_part).

## Part 2: Automation, Containerization & Cluster Execution

The second part focused on automating the complete workflow, from compilation and testing to containerized execution on the **CINECA G100 cluster**.

### Workflow Automation

A GitHub Actions workflow was developed to:

1. Build and compile the project.
2. Run the complete test suite.
3. Build a **Singularity** container containing the required dependencies.
4. Transfer the container and required files to the CINECA G100 cluster.
5. Submit the computation through **SLURM**.

### Containerization with Singularity

The execution environment is defined through the `Singularity.def` file, providing a reproducible container containing the dependencies required by the image-conversion program.

### Secure Cluster Authentication

Access to the CINECA cluster was managed through **GitHub Secrets**, allowing SSH credentials to be used by the automated workflow without exposing them in the repository.

Since the cluster authentication relied on short-lived certificates, credentials had to be periodically renewed during development and testing.

### Job Submission with SLURM

Cluster jobs are submitted through the `job.sh` script using **SLURM**.

The job configuration controls aspects such as:

- number of nodes and tasks;
- execution time limits;
- output redirection;
- resource allocation and monitoring.

### Example Image Processing

The `random_gen_images.py` script was used to generate random `.ppm` images for larger-scale tests.

The generated images were processed on the cluster using the grayscale conversion utility, with resulting `.pgm` files stored in the `results` directory.

## Continuous Integration

Compilation and testing are automated through **GitHub Actions**, allowing the project to be validated consistently whenever changes are pushed to the repository.

The overall workflow combines:

- automated testing;
- continuous integration;
- Singularity containerization;
- secure remote deployment;
- SLURM job scheduling;
- HPC cluster execution.

## Collaboration

All team members contributed to the design of the testing strategy and to the implementation and troubleshooting of the automated workflow.

## License

This project is licensed under the [MIT License](./LICENSE).
