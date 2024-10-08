#!/bin/bash
function run_diff_test {
    ./ft_nm $EXECUTABLE $1 > result_ft_nm.txt
    nm $EXECUTABLE $1 > result_nm.txt
    echo -n "$1 "
    diff result_ft_nm.txt result_nm.txt 2>&1 >/dev/null && echo -ne "\033[32mOK\033[0m " || echo -ne "\033[31mKO\033[0m "
    rm result_ft_nm.txt result_nm.txt
}

function run_all_tests {
    EXECUTABLE="$1"
    echo "TEST $EXECUTABLE"
    run_diff_test ""

    run_diff_test "-a"
    run_diff_test "-g"
    run_diff_test "-r"
    run_diff_test "-u"
    run_diff_test "-p"
    echo ''

    run_diff_test "-ra"
    run_diff_test "-rg"
    run_diff_test "-ru"
    run_diff_test "-rp"
    run_diff_test "-pa"
    run_diff_test "-pg"
    run_diff_test "-pr"
    run_diff_test "-pu"
    run_diff_test "-ag"
    run_diff_test "-ar"
    run_diff_test "-au"
    run_diff_test "-ap"
    run_diff_test "-ga"
    run_diff_test "-gr"
    run_diff_test "-gu"
    run_diff_test "-gp"
    echo ''

    run_diff_test "-arg"
    run_diff_test "-aru"
    run_diff_test "-arp"
    run_diff_test "-agu"
    run_diff_test "-agp"
    run_diff_test "-aup"
    run_diff_test "-rag"
    run_diff_test "-rau"
    run_diff_test "-rap"
    run_diff_test "-rgu"
    run_diff_test "-rgp"
    run_diff_test "-rup"
    run_diff_test "-gar"
    run_diff_test "-gau"
    run_diff_test "-gap"
    run_diff_test "-gru"
    run_diff_test "-grp"
    run_diff_test "-gup"
    run_diff_test "-uar"
    run_diff_test "-uag"
    run_diff_test "-uap"
    run_diff_test "-urg"
    run_diff_test "-urp"
    run_diff_test "-ugp"
    run_diff_test "-par"
    run_diff_test "-pag"
    run_diff_test "-pau"
    run_diff_test "-prg"
    run_diff_test "-pru"
    run_diff_test "-pgu"
    echo ''

    run_diff_test "-gurp"
    run_diff_test "-aurp"
    run_diff_test "-agrp"
    run_diff_test "-agup"
    run_diff_test "-agur"
    echo ''

    run_diff_test "-agurp"
}

run_all_tests "ft_nm"
run_all_tests "tests_binaries/minishell"
run_all_tests "tests_binaries/philo"
