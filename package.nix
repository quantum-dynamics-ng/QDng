{
  lib,
  stdenv,
  autoreconfHook,
  gfortran,
  fftw,
  protobuf,
  blas,
  lapack,
  zlib,
  bzip2,
  libxml2,
  flex,
  bison,
}:

stdenv.mkDerivation (finalAttrs: {
  pname = "qdng";
  version = "1.0.0";

  src = builtins.filterSource (path: type: path != ".git" && path != "result" && path != "build") ./.;

  configureFlags = [
    "--enable-openmp"
    "--with-blas=-lblas"
    "--with-lapack=-llapack"
    "--disable-gccopt"
  ];

  enableParallelBuilding = true;

  nativeBuildInputs = [
    autoreconfHook
    gfortran
    flex
    bison
  ];

  buildInputs = [
    fftw
    protobuf
    blas
    lapack
    bzip2
    zlib
    libxml2
  ];

  meta = {
    description = "Molecular wavepacket dynamics package";
    homepage = "https://github.com/quantum-dynamics-ng/QDng";
    platforms = lib.platforms.linux;
    maintainer = [ lib.maintainers.markuskowa ];
    license = lib.licenses.gpl3Only;
  };
})
