# QDng
## General purpose program package for molecular quantum dynamics

Features:

* Time propagation
* Eigenfunction calculations
* Optimal control theory
* Grid based dynamics
* Fourier transform based derivates
* Nonadiabatic couplings
* Multiple electronic states
* Composable matrix Hamiltionians and operators
* Time dependent laser fields

## Running under the program Nix

QDng is packaged with [NixOS-QChem](https://github.com/Nix-QChem/NixOS-QChem) and can be used directly through the overlay or the flake.

[![built with nix](https://builtwithnix.org/badge.svg)](https://builtwithnix.org)

## Building

The following depencies is required:
 * google protobuf (<= 3.21)
 * libxml2
 * lapack
 * blas
 * fftw3 (with openmp support)

 For Wavefunction compression
 * zlib (optional)
 * libbzip (optional)

Use ./genbs to generate the build system (configure) after a fresh git clone.
This is not need for the release tarball (call configure direcltly).


## Citation

Cite as:
M. Kowalewski, R. de Vivie-Riedle, "QDng: A Grid Based Molecular Quantum Dynamics Package", Zenodo. https://doi.org/10.5281/zenodo.10944497

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.10944496.svg)](https://doi.org/10.5281/zenodo.10944496)

