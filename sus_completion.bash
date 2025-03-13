_comp_cmd_sus() {
    local cur prev words cword
    _init_completion || return

    local PATH="$PATH:/sbin:/usr/sbin:/usr/local/sbin"

    # Long incomplete arguments (--us) and --user=... coming soon...........
    
    # Subcommands completion
    for ((i = 1; i <= cword; i++)); do
        if [[ ${words[i]} != -* ]]; then
            local PATH="$PATH:/sbin:/usr/sbin:/usr/local/sbin"
            local _comp_root_command=$1
            _comp_command_offset $i
            return
        fi
        [[ ${words[i]} == @(--@(user)|-u) ]] && ((i++))
    done

    # Users completion (-u user, --user user) 
    if [[ "$prev" == "-u" || "$prev" == "--user" ]]; then
        COMPREPLY=( $( compgen -u -- "$cur" ) )
        return 0
    fi
    
    # Short user arg (-uuser)
    if [[ "$cur" =~ ^-u[^[:space:]]+$ ]]; then
        local prefix="-u"
        local arg="${cur#-u}"
        COMPREPLY=( $( compgen -u -- "$arg" ) )
        local i
        for i in "${!COMPREPLY[@]}"; do
            COMPREPLY[$i]="${prefix}${COMPREPLY[$i]}"
        done
        return 0
    fi
    
    # Long args
    if [[ "$cur" == --* ]]; then
        COMPREPLY=( $( compgen -W "--help --version --user" -- "$cur" ) )
        return 0
    fi

    # Short args 
    if [[ "$cur" == -* ]]; then
        COMPREPLY=( $( compgen -W "-u -h -v" -- "$cur" ) )
        return 0
    fi

    # Nothing
    if [[ "$prev" =~ ^(-h|--help|-v|--version)$ ]]; then
        return 0
    fi
    
    # Commands completion
    COMPREPLY=( $( compgen -c -- "$cur" ) )
    return 0
}
complete -F _comp_cmd_sus sus
