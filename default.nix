
let
  version = "dev";
  pkgs = import <nixpkgs> {};

in pkgs.callPackage ./package.nix { protobuf = pkgs.protobuf3_21; }
