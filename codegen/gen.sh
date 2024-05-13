if [ -f .generated ]
then
    echo 'Grammar already generated, remove ".generated" file for rerun'
    exit 0
fi

root_dir=$1

antlr4 -Dlanguage=Cpp $root_dir/grammar/GoLexer.g4 $root_dir/grammar/GoParser.g4 -visitor -no-listener -o output
mv output/*.h $root_dir/include/antlr4/codegen
mv output/*.cpp $root_dir/src/antlr4/codegen

rm -fr output

touch .generated
