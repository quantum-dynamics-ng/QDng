{
  description = "Quantum dynamics program package";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-24.11";
  };

  outputs = { self, nixpkgs }: {

    packages.x86_64-linux = let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
      inherit (pkgs) lib callPackage;
    in {
      qdng = callPackage ./package.nix { protobuf = pkgs.protobuf3_21; };
      default = self.packages.x86_64-linux.qdng;
    };

    hydraJobs.x86_64-linux = {
      inherit (self.packages.x86_64-linux) qdng;
    };
  };
}
