set foldmethod=syntax

" enables syntax highlighting
syn on

" sets format options in INSERT mode
"  c - Automatically wrap comments.  Insert the comment leader automatically.
"  r - Insert comment leader in a comment when a new line is inserted.
"  t - Automatically wrap text when the textwidth value is reached.  If
"      textwidth is not defined, text will not be wrapped.
set formatoptions=crt
"set textwidth=80

" determines how far something shifts when you use >> or <<
set shiftwidth=4

" determines how many spaces your tabs shift text on your screen
set tabstop=4

" copies the indent from the previous line when you insert a new line
" below it
set autoindent

" makes Vim show you line numbers on the left
set number

" use only spaces instead of tab characters when you hit <tab>
"set expandtab

" Windows *********************************************************************
set equalalways " Multiple windows, when created, are equal in size
set splitbelow splitright

" Vertical and horizontal split then hop to a new buffer
:noremap <Leader>v :vsp<CR>
:noremap <Leader>h :split<CR>

" Open cur directory
:noremap <Leader>e :e.<CR>

" Rapid quit
:noremap <Leader>q :qa<CR>

" Fold option
:noremap <Leader>z zi<CR>

" Scroll window up and down like a browser
map <s-Down> <C-E>
map <s-Up> <C-Y>

" Switch windows right to left
map <s-Left> <C-W>W
map <s-Right> <C-W>w

" CSCOPE map
source ~/.vim/.cscope_maps.vim

" Minibuffer
source ~/.vim/minibufexpl.vim

" Always on when >= 2 files
let g:miniBufExplorerMoreThanOne=2

" Vertical Split
let g:miniBufExplSplitBelow=0
let g:miniBufExplVSplit = 20

" Sessions ********************************************************************
" Sets what is saved when you save a session
set sessionoptions=blank,buffers,curdir,folds,help,resize,tabpages,winsize

" Save session 1/2/3/4
:noremap <Leader>s1 :CMiniBufExplorer<cr>:mksession! .vim_s1<cr>:MiniBufExplorer<CR>
:noremap <Leader>s2 :CMiniBufExplorer<cr>:mksession! .vim_s2<cr>:MiniBufExplorer<CR>
:noremap <Leader>s3 :CMiniBufExplorer<cr>:mksession! .vim_s3<cr>:MiniBufExplorer<CR>
:noremap <Leader>s4 :CMiniBufExplorer<cr>:mksession! .vim_s4<cr>:MiniBufExplorer<CR>

" Load saved session 1/2/3/4
:noremap <Leader>l1 :CMiniBufExplorer<cr>:source .vim_s1<cr>:MiniBufExplorer<CR>
:noremap <Leader>l2 :CMiniBufExplorer<cr>:source .vim_s2<cr>:MiniBufExplorer<CR>
:noremap <Leader>l3 :CMiniBufExplorer<cr>:source .vim_s3<cr>:MiniBufExplorer<CR>
:noremap <Leader>l4 :CMiniBufExplorer<cr>:source .vim_s4<cr>:MiniBufExplorer<CR>






