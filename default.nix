
let
  version = "dev";
  pkgs = import <nixpkgs> {};
  protobuf = pkgs.protobuf3_11;

in with pkgs; stdenv.mkDerivation {
  pname = "qdng";
  inherit version;

  src = builtins.filterSource (path: type: path != ".git" && path != "result" && path != "build") ./.;

  configureFlags = [
    "--enable-openmp"
    "--with-blas=-lblas"
    "--with-lapack=-llapack"
  ];

  enableParallelBuilding = true;

  preConfigure = ''
    ./genbs
  '';

  buildInputs = [ fftw protobuf blas lapack
                  bzip2 zlib libxml2 flex bison ];
  nativeBuildInputs = [ automake autoconf libtool gfortran ];

  meta = with lib; {
    description = "Quantum dynamics program package";
    platforms = platforms.linux;
    maintainer = [ maintainers.markuskowa ];
    license = licenses.unfree;
  };
}
