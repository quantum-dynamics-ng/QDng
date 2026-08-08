{
  description = "Quantum dynamics program package";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }: let
   forAllSystems = function:
      nixpkgs.lib.genAttrs [
        "x86_64-linux"
        "aarch64-linux"
      ] (system: function nixpkgs.legacyPackages.${system} system);

  in {

    packages = forAllSystems(pkgs: system:
    let
      pkgs = nixpkgs.legacyPackages.${system};
      inherit (pkgs) callPackage;
    in {
      qdng = callPackage ./package.nix { };
      default = self.packages.x86_64-linux.qdng;
      handbook = callPackage ./handbook.nix { qdng = self.packages.${system}.qdng ;};
    });

    hydraJobs.forAllSystems = (pkgs: system: {
      inherit (self.packages.${system}) qdng;
    });
  };
}
