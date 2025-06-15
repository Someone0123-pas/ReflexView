{
  description = "Nix-Flake for ReflexView";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-24.11";
  };

  outputs = inputs@{self, nixpkgs, ...}:
  let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages."${system}";
    qt = with pkgs.qt6; env "qt-${qtbase.version}" [
      qtwayland
      qtdeclarative
    ];
  in {
    devShells."${system}".default = pkgs.mkShell {
      QTPATH = qt;
      packages = (with pkgs; [
	      bashInteractive
        cmake
        ninja
	      gdb
	      clang-tools

	      qtcreator
	      libGL
      ]) ++ [ qt ];
    };
  };
}
