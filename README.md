# QDng
## General purpose program package for molecular quantum dynamics

Features:

* Time propagation
* Eigenfunction calculations
* Optimal control theory
* Grid based dynamics
* Fourier transform based derivates
* Composable matrix Hamiltionians and operator
* Time dependent laser fields

Molecular quantum

## Building

The following depencies is required:
 * google protobuf
 * libxml2
 * lapack
 * blas
 * fftw3 (with openmp support)

 For Wavefunction compression
 * zlib (optional)
 * libbzip (optional)

Use ./genbs to generate the build system (configure) after a fresh git clone.
This is not need for the release tarball (call configure direcltly).

## Running under Nix

QDng is packaged with [NixOS-QChem](https://github.com/Nix-QChem/NixOS-QChem) and can be used directly through the overlay.



