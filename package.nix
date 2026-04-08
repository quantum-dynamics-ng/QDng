{ lib, stdenv, fetchurl, requireFile, gfortran, fftw, protobuf
, blas, lapack
, automake, autoconf, libtool, zlib, bzip2, libxml2, flex, bison,
  autoreconfHook
} :

stdenv.mkDerivation rec {
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

  buildInputs = [ fftw protobuf blas lapack
                  bzip2 zlib libxml2 flex bison ];
  nativeBuildInputs = [ autoreconfHook gfortran ];

  meta = with lib; {
    description = "Quantum dynamics program package";
    platforms = platforms.linux;
    maintainer = [ maintainers.markuskowa ];
    license = licenses.gpl3Only;
  };
}

