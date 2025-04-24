use std::process::Command;
use std::path::Path;

fn main() {
    // Set paths, both are not used
    let _c_dir = Path::new("C");
    let _output_dir = Path::new(".");

    // ---------------------
    // Compile start.cpp -> start.dll + start.lib
    // ---------------------
    let status_start = Command::new("cl.exe")
        .args(&[
            "/LD",
            "..\\C\\start.cpp",
            "/DGRAMMAR_END_OF_TOKEN_MARKER=\" \"",
            "/DGRAMMAR_END_OF_TOKEN_MARKER_SIZE=1",
            "/link",
            "/OUT:start.dll",
            "/IMPLIB:start.lib",
        ])
        .status()
        .expect("Failed to compile start.cpp");

    assert!(status_start.success(), "Failed to compile start.dll");

    // ---------------------
    // Compile clap.c -> clap.dll + clap.lib (linking against start.lib)
    // ---------------------
    let status_clap = Command::new("cl.exe")
        .args(&[
            "/LD",
            "..\\C\\clap.c",
            "/link",
            "/OUT:clap.dll",
            "/IMPLIB:clap.lib",
            "start.lib", // Link to the start.lib
        ])
        .status()
        .expect("Failed to compile clap.c");

    assert!(status_clap.success(), "Failed to compile clap.dll");

    // Link the resulting clap.lib in Rust
    println!("cargo:rustc-link-search=native=.");
    println!("cargo:rustc-link-lib=dylib=clap");
}

