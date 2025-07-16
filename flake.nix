{
  description = "Asset-Viewer for Kirby and the Amazing Mirror";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-25.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = inputs@{self, nixpkgs, flake-utils}: flake-utils.lib.eachDefaultSystem (system:
  let
    pkgs = import nixpkgs { inherit system; };

    revision = with self; if sourceInfo?dirtyShortRev
      then sourceInfo.dirtyShortRev
      else sourceInfo.shortRev;

    reflexview = pkgs.stdenv.mkDerivation {
      pname = "ReflexView";
      version = "0.0.${revision}";
      src = ./.;

      nativeBuildInputs = with pkgs; [
        cmake
        ninja
        qt6.wrapQtAppsHook
      ];

      buildInputs = with pkgs; [
        qt6.qtbase
        qt6.qtwayland
        qt6.qtdeclarative
        wayland
        libGL
      ];
    };
  in {    
    packages.default = reflexview;

    apps.default = flake-utils.lib.mkApp {
      drv = [ self.packages.${system}.default ];
    };

    devShells.default = pkgs.mkShell {
      inputsFrom = [ self.packages.${system}.default ];

      packages = with pkgs; [
	      bashInteractive
	      qt6.qttools
	      gdb
	      clang-tools
	      qtcreator
      ];
    };
  });
}
