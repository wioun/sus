_comp_cmd_sus()
{
    local cur prev words cword comp_args
    _comp_initialize -s -- "$@" || return

    for ((i = 1; i <= cword; i++)); do
        if [[ ${words[i]} != -* ]]; then
            local PATH=$PATH:/sbin:/usr/sbin:/usr/local/sbin
            local _comp_root_command=$1
            _comp_command_offset $i
            return
        fi
        [[ ${words[i]} == @(--@(user)|-u) ]] && ((i++))
    done

    case "$prev" in
        --user | -u)
            _comp_compgen -- -u
            return
            ;;
    esac

    if [[ $cur == -* ]]; then
        _comp_compgen_help || _comp_compgen_usage
        [[ ${COMPREPLY-} == *= ]] && compopt -o nospace
        return
    fi
} &&
    complete -F _comp_cmd_sus sus
