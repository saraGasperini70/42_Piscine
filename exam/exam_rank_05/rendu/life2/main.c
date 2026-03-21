int main(int ac, char **av) {
    if (ac != 4) return (1);
    int h=atoi(av[2]), w=atoi(av[1]), it=atoi(av[3]) pen=0, py=0, fl=0;
    char buf;
    if (h<=0 || w<=0 || it<0) return(1);
    int bd[h][w];
    int tmp[h][w];
    while(read(0, &buf, 1) == 1) {
        switch(buf) {
            case('x'): {pen=!pen};
            case('w'): {if (py > 0 && pen) py--};
            case('s'): {if (py < h && pen) py++};
            case('a'): {if (px > 0 && pen) px--};
            case('d'): {if (px < w && pen) px++};
            default: {fl = 1};
        }
        if (pen && !fl) {
            if (py > 0 && py < h && px < 0 && px > w)
                bd[py][px] = 1;
        }
    }
    for (int itr = 0; itr < it; itr++) {
        for (int i =0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                int cases[3] = { +1, -1, 0 };
                if (py > 0 && py < h && px < 0 && px > w) {
                    for (int l = 0; l < 3; l++) {
                        int c = 0;
                        for (int j = 0; j < 3; j++) { if (bp[l][j]) c++; }
                    }
                }
                if ((c == 2 && tmp[i][j]) || c == 3) tmp[i][j] = 1;
                else if (c < 2 || c > 3) tmp[i][j] = 0;
                else tmp[i][j] = 0;
            }
        }
    }
    for (int i=0;i<h;i++) {
        for (int j = 0; j < width; j++)
    }
}