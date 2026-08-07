{
  lib,
  stdenv,
  autoreconfHook,
  bison,
  flex,
  gfortran,
  blas,
  bzip2,
  fftw,
  lapack,
  libxml2,
  protobuf_21,
  zlib,
}:

stdenv.mkDerivation (finalAttrs: {
  pname = "qdng";
  version = "develop";

  src = builtins.filterSource (path: type: path != ".git" && path != "result" && path != "build") ./.;

  configureFlags = [
    "--enable-openmp"
    "--disable-gccopt"
  ];

  enableParallelBuilding = true;

  nativeBuildInputs = [
    autoreconfHook
    bison
    flex
    gfortran
  ];

  buildInputs = [
    blas
    bzip2
    fftw
    lapack
    libxml2
    protobuf_21
    zlib
  ];

  meta = {
    description = "Molecular wavepacket dynamics package";
    homepage = "https://github.com/quantum-dynamics-ng/QDng";
    platforms = lib.platforms.linux;
    maintainers = [ lib.maintainers.markuskowa ];
    license = lib.licenses.gpl3Only;
  };
})
