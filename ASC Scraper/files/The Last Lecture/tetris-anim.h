#ifndef __TETRIS_ANIM__
#define __TETRIS_ANIM__

#include <simplecpp>
#include "tetris-board.h"

class infoPanel {
 double centrex, centrey;
 //double width, height;
 Rectangle panel;
 Text infoText;
 int score, xrows; // to store from the last update
public:
 infoPanel(double x, double y, double w, double h, Color c) 
  : centrex(x), centrey(y), // width(w), height(h),
  panel(x,y,w,h), infoText(x,y,"0 [ 0 ]") 
 {
  panel.setColor(c);
  panel.setFill();
 }

 void updateInfo(const string& txt) {
  infoText.reset(centrex, centrey, txt.c_str());
 }

    void showScore() {
  string txt = std::to_string(score) + " [ " + std::to_string(xrows) + " ]";
  updateInfo(txt);
 }

    void updateScore(int sr, int xr) {
  score = sr; xrows = xr;
  showScore();
 }

};

class boardDrawer {
 double margin = 5;
 string marginColor = "gray";
 int nBlink = 1;
 double blinkTime = 0.2;
 Color blinkOnColor, blinkOffColor;

    const board* Brd; // will only use nrows(), ncols() and at() 
    int nrows, ncols;
    double boxsz, height, width;
    vector<vector<Rectangle*>> boxes; // for each cell, the coloured box
 Rectangle* window;
 infoPanel* info;

    Rectangle* mkBox(int row, int col) {
        return new Rectangle(margin+(col+0.5)*boxsz,margin+height-(row+0.5)*boxsz,boxsz,boxsz);
    }

    Color getColor(int x, int y) {
        static const char* colours[] = { "black", "cyan", "white", "yellow", "blue", "magenta", "green", "red"};
        return COLOR(colours[Brd->at(x,y)]);
    }

public:
    boardDrawer (double h, int nhide, const board* B)
                : Brd(B), nrows(Brd->nrows() - nhide), ncols(Brd->ncols()),
                  boxsz(h/nrows), height(h), width(boxsz*ncols),
                  boxes(nrows,vector<Rectangle*>(ncols,nullptr)) {
  double infoWidth = width, infoHeight = boxsz;
  double winwidth = width + 2*margin;
  double winheight = height + 3*margin + infoHeight;

        initCanvas("Tetris",winwidth,winheight);
  blinkOnColor = COLOR("gray");
  blinkOffColor = COLOR("black");
  beginFrame();
  window = new Rectangle(winwidth/2, winheight/2, winwidth - 2*margin, winheight - 2*margin);
  info= new infoPanel(winwidth/2, winheight - (margin+infoHeight/2), infoWidth, infoHeight, COLOR("gray"));
        for(int y=0; y<boxes.size(); y++) {
            for(int x=0; x<boxes[y].size(); x++) {
                boxes[y][x] = mkBox(y,x);
                boxes[y][x]->setColor(getColor(x,y));
                boxes[y][x]->setFill();
            }
        }
  endFrame();
    }

 ~boardDrawer() {
        for(int y=0; y<boxes.size(); y++)
            for(int x=0; x<boxes[y].size(); x++)
    delete boxes[y][x];
  delete info;
  delete window;
 }

    void refresh() {
  beginFrame();
        for(int y=0; y<boxes.size(); y++) {
            for(int x=0; x<boxes[y].size(); x++) {
                boxes[y][x]->setColor(getColor(x,y));
            }
        }
  endFrame();
    }

    void update(const vector<point>& L) {
  beginFrame();
        for(auto& p: L) {
            int x = p.X(), y = p.Y();
            if(y>=nrows) continue;
            boxes[y][x]->setColor(getColor(x,y));
        }
  endFrame();
    }

    void updateBlock(const block& piece) {
        update(vector<point>(piece.where()));
    }

 void blink(vector<int> rows) {
  for(int i=0; i < 2*nBlink+1; ++i) {
   Color clr = (i%2) ? blinkOffColor : blinkOnColor;
   beginFrame();
   for(auto& y: rows) {
    for(int x=0; x<boxes[y].size(); ++x) {
     boxes[y][x]->setColor(clr);
    }
   }
   endFrame();
   wait(blinkTime);
  }
 }

    void showScore() {
  info->showScore();
 }

    void updateScore(int score, int xrows) {
  info->updateScore(score,xrows);
 }

    void updateInfo(const string& txt) {
  info->updateInfo(txt);
 }
};

#endif // __TETRIS_ANIM__