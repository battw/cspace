def Settings( **kwargs ):
    return { 'flags': 
            [ '-lncurses'
                , '-Wall'
                , '-Wextra'
                # , 'Werror'
                , 'std=c11'
                , 'c'  
                ] }

