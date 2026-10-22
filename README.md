# NASweep: Local AI Storage Cleaner for Your NAS

![NASweep](assets/thumbnail.png)

A lightweight C daemon that runs 24/7 on your NAS, finds duplicate and blurry
files, and uses local AI to explain what's safe to delete. Private by design:
no cloud, nothing leaves home.

Built for the **UGREEN Hackathon 2026 on AI NAS**, track *Automate Your Workflow*.

## How it works

1. **Scan**: walk the share with `nftw()` (symlinks are not followed).
2. **Group by size**: only files of the same size can be duplicates, so most files are never read.
3. **Hash**: FNV-1a 64-bit over the contents of same-size files.
4. **Confirm**: byte-for-byte compare, so a hash collision can never cause a false match.
5. **Explain** *(in progress)*: send the report to a local LLM (llama.cpp / Ollama) for a plain-language summary.

```
src/
  main.c    CLI, text + JSON output
  scan.c    recursive directory walk
  hash.c    FNV-1a hashing, byte compare
  dupes.c   size -> hash -> confirm pipeline
  ai.c      local LLM client (stub)
tests/
  run_tests.sh   end-to-end tests
```

## Build and run

```sh
make
./build/nasweep /path/to/share
./build/nasweep --json /path/to/share
./build/nasweep --ai http://localhost:8080/v1/chat/completions /path/to/share
make test
```

### Docker (on the NAS)

```sh
docker build -t nasweep .
docker run --rm -v /volume1/photos:/data:ro nasweep
```

## Roadmap

- [x] Exact duplicate detection (size, hash, byte compare)
- [x] Text and JSON reports
- [ ] Local LLM explanations via llama.cpp / Ollama (`src/ai.c`)
- [ ] Near-duplicate and blurry photo detection (perceptual hash)
- [ ] Daemon mode: rescan on a schedule, notify on new waste
- [ ] Safe cleanup: move to a quarantine folder instead of deleting
- [ ] Small web UI for reviewing groups
