noremap <leader>pr :w<cr>:!./compile.sh && ./set_ptrace_scope.sh 0 && ./arcade; ./set_ptrace_scope.sh 1<cr>
noremap <leader>ps :w<cr>:!./runscratch.sh<cr>
noremap <leader>pes :w<cr>:edit scratch.c<cr>
