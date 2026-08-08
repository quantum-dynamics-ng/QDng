
let
  version = "dev";
  pkgs = import <nixpkgs> {};

in pkgs.callPackage ./package.nix { }
