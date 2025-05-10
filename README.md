# dns-resolver

Resolve hostnames (IPv4 only) listed in one or more text files and log the results.

```bash
./pa5_lookup <log_file> <datafile1> [datafile2 …]
```

* **Input:** each data file contains one hostname per line  
* **Output:** `host, IPv4 | NOT_RESOLVED`  
* Accepts up to **64** data files per run (`MAX_DATAFILES`)

### Build

```bash
gcc -std=c11 -Wall pa5_lookup.c -o pa5_lookup
```

### Example
```bash
./pa5_lookup log.txt hosts.txt
# ./pa5_lookup: total time is 0.012345 seconds
cat log.txt
example.com, 93.184.216.34
ipv6only.example, NOT_RESOLVED
```
