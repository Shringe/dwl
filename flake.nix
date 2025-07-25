{
  inputs = {
    nixpkgs = { url = "github:NixOS/nixpkgs/nixos-unstable"; };
    flake-utils = { url = "github:numtide/flake-utils"; };

    scenefx = {
      url = "github:wlrfx/scenefx";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = { self, nixpkgs, ... }@inputs: 
    let
      systemOutputs = inputs.flake-utils.lib.eachDefaultSystem (system:
        let
          pkgs = import nixpkgs { inherit system; };
          wlroots_package = pkgs.wlroots_0_19;
          enable_xwayland = true;

          baseDeps = with pkgs; [
            libinput
            wayland
            wlroots_package
            libxkbcommon
            wayland-protocols
            pkg-config
            wayland-scanner
            pixman

            # Patches
            ## SceneFX
            # scenefx # outdated
            inputs.scenefx.packages.${system}.default
            libGL
          ];

          xwaylandDeps = with pkgs; [
            xorg.libxcb
            xorg.xcbutilwm
            xwayland
          ];

          buildDeps = baseDeps ++ (if enable_xwayland then xwaylandDeps else []);
        in rec {
          overlay = self: super: {
            dwl = packages.dwl;
          };

          devShells.default = pkgs.mkShell {
            name = "dwl";
            buildInputs = buildDeps;
            packages = [ packages.dwl ];
          };

          packages.dwl = pkgs.stdenv.mkDerivation (finalAttrs: {
            pname = "dwl";
            version = "main";
            src = self;

            postInstall = ''
              mkdir -p $out/share/wayland-sessions
              cp ./dwl.desktop $out/share/wayland-sessions/dwl.desktop
            '';

            passthru.providedSessions = [ "dwl" ];

            nativeBuildInputs = buildDeps;
            buildInputs = buildDeps;

            outputs = [
              "out"
              "man"
            ];

            makeFlags = [
              "PKG_CONFIG=${pkgs.stdenv.cc.targetPrefix}pkg-config"
              "WAYLAND_SCANNER=wayland-scanner"
              "PREFIX=$(out)"
              "MANDIR=$(man)/share/man"
            ] ++ (if enable_xwayland then [
              ''XWAYLAND="-DXWAYLAND"''
              ''XLIBS="xcb xcb-icccm"''
            ] else []);

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
    in systemOutputs // {
      overlays.default = final: prev: {
        dwl = systemOutputs.packages.${prev.system}.dwl;
      };
    };
}
