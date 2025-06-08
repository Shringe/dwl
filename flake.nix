{
  inputs = {
    nixpkgs = { url = "github:NixOS/nixpkgs/nixos-unstable"; };
    flake-utils = { url = "github:numtide/flake-utils"; };
  };

  outputs = { nixpkgs, ... }@inputs: inputs.flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs { inherit system; };

      buildDeps = with pkgs; [
        gdb
        lldb
        libinput
        xorg.libxcb
        libxkbcommon
        pixman
        wayland
        wayland-protocols
        wlroots
        xorg.libX11
        xorg.xcbutilwm
        xwayland
        pkg-config
        wayland-scanner
      ];
    in rec {
      overlay = self: super: {
        sdwl = packages.dwl;
      };

      devShells.default = pkgs.mkShell {
        name = "dwl";
        buildInputs = buildDeps;
        packages = [ packages.dwl ];
      };

      packages.dwl = pkgs.stdenv.mkDerivation (finalAttrs: {
        pname = "dwl";
        version = "unstable";
        src = ./.;

        nativeBuildInputs = with pkgs; [
          installShellFiles
          pkg-config
          wayland-scanner
        ];

        buildInputs = with pkgs; [
          libinput
          xorg.libxcb
          libxkbcommon
          pixman
          wayland
          wayland-protocols
          wlroots
          xorg.libX11
          xorg.xcbutilwm
          xwayland

          xdg-desktop-portal
          xdg-desktop-portal-wlr
          xdg-desktop-portal-gtk
        ];

        outputs = [
          "out"
          "man"
        ];

        makeFlags = [
          "PKG_CONFIG=${pkgs.stdenv.cc.targetPrefix}pkg-config"
          "WAYLAND_SCANNER=wayland-scanner"
          "PREFIX=$(out)"
          "MANDIR=$(man)/share/man"
          ''XWAYLAND="-DXWAYLAND"''
          ''XLIBS="xcb xcb-icccm"''
        ];

        strictDeps = true;

        # required for whitespaces in makeFlags
        __structuredAttrs = true;

        meta = {
          homepage = "https://codeberg.org/dwl/dwl";
          description = "Dynamic window manager for Wayland";
          inherit (pkgs.wayland.meta) platforms;
          mainProgram = "dwl";
        };
      });

      packages.default = packages.dwl;
    }
  );
}
