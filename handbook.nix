{ lib,
  stdenvNoCC,
  qdng,
  texliveFull,
}:

stdenvNoCC.mkDerivation {
  pname = "${qdng.pname}-handbook";
  inherit (qdng) version src;

  nativeBuildInputs = [ texliveFull ];

  buildPhase = ''
    cd doc

    pdflatex Handbook.tex
    bibtex Handbook
    pdflatex Handbook.tex
    pdflatex Handbook.tex
  '';

  installPhase = ''
    mkdir -p $out
    cp Handbook.pdf $out/
  '';
}
