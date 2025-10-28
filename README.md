// TO BUILD:

cmake -B build/debug
cmake --build build/debug


// TO RUN:

build\bin\maratona_score.exe convert-batch D:\MaratonaScore\examples\batch-conversions.txt

build\bin\maratona_score.exe batch D:\MaratonaScore\examples\batch-processing.txt

build\bin\maratona_score.exe convert-batch D:\MaratonaScore\examples\batch-conversions.txt && build\bin\maratona_score.exe batch D:\MaratonaScore\examples\batch-processing.txt