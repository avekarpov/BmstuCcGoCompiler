root_dir=$1

echo $root_dir

antlr4 -Dlanguage=Cpp $root_dir/grammar/GoLexer.g4 $root_dir/grammar/GoParser.g4 -visitor -no-listener -o output
mv output/*.h $root_dir/include/antlr4/codegen
mv output/*.cpp $root_dir/src/antlr4/codegen

rm -fr output
