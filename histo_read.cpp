
#include "histo_read.h"
histo_read::histo_read(readBeam * beams, string outfile) {

  //create root file
  file_read = new TFile (outfile.c_str(),"RECREATE");
  file_read->cd();
  for(unsigned int k = 0; k < beams->pb.size(); k++){
    char iso[16];
    char reac[16];
    int tmpZ = beams->GetZ(beams->pb.at(k));
    int tmpA = beams->GetA(beams->pb.at(k));
    sprintf(iso,"%i%s",tmpA,beams->isotope(tmpZ).c_str());
    dirCorrPB[k] = file_read->mkdir(Form("%s",iso));
    dirCorrPB[k]->cd();
    for(int j = 0; j < 6; j++) {
      if(j < 4) sprintf(reac,"%ip", j + 1);
      else if (j == 4) sprintf(reac,"1a");
      else if (j == 5) sprintf(reac,"1d");
      dirCorrP[k][j] = dirCorrPB[k]->mkdir(Form("%s_decay", reac));
      dirCorrP[k][j]->cd();
      for(unsigned int i = 0; i < beams->sb[k].size(); i++) {
        tmpZ = beams->GetZ(beams->sb[k].at(i));
        tmpA = beams->GetA(beams->sb[k].at(i));
        if(j < 4) sprintf(iso,"%i%s",(tmpA + (j+1)),beams->isotope(tmpZ+(j+1)).c_str());
        else if (j == 4) sprintf(iso,"%i%s",(tmpA + 4),beams->isotope(tmpZ+2).c_str());
        else if (j == 5) sprintf(iso,"%i%s",(tmpA + 2),beams->isotope(tmpZ+1).c_str());
        dirCorrI[k][j][i] = dirCorrP[k][j]->mkdir(Form("%s",iso));
        dirCorrI[k][j][i]->cd();
        TH1I *h1 = new TH1I(Form("Erel_%s_%s%s",iso,reac,beams->sb[k].at(i).c_str()),"",800,0,17); ErelList[k][j].push_back(h1);
        TH1I *h2 = new TH1I(Form("Ex_%s_%s%s",iso,reac,beams->sb[k].at(i).c_str()),"",800,-2,15); ExList[k][j].push_back(h2);
        TH1I *h3 = new TH1I(Form("ThetaCM_%s_%s%s",iso,reac,beams->sb[k].at(i).c_str()),"",200,0,10); TCMList[k][j].push_back(h3);
        TH1I *h4 = new TH1I(Form("VCM_%s_%s%s",iso,reac,beams->sb[k].at(i).c_str()),"",100,0,14); VCMList[k][j].push_back(h4);
        TH1I *h5 = new TH1I(Form("Egam_%s_%s%s",iso,reac,beams->sb[k].at(i).c_str()),"",800,0,17); EgamList[k][j].push_back(h5);
        TH2I *hh1 = new TH2I(Form("EgamVsErel_%s_%s%s",iso,reac,beams->sb[k].at(i).c_str()),"",800,0,17,800,0,17); EgamErelList[k][j].push_back(hh1);
        TH2I *hh2 = new TH2I(Form("ErelTheta_%s_%s%s",iso,reac,beams->sb[k].at(i).c_str()),"",300,0,30,400,0,17); EthetaList[k][j].push_back(hh2);
        TH2I *hhh = new TH2I(Form("ErelCos_%s_%s%s",iso,reac,beams->sb[k].at(i).c_str()),"",100,0,1,400,0,17); ErelCosList[k][j].push_back(hhh);
        TH1I *h6 = new TH1I(Form("LongMom_%s_%s%s",iso,reac,beams->sb[k].at(i).c_str()),"",2000,0,20000); MomList[k][j].push_back(h6);
        if(j == 1){
          TH2I *hh2 = new TH2I(Form("Erel_VsPro_%s_%s%s",iso,reac,beams->sb[k].at(i).c_str()),"",800,0,17,800,0,17); ErelVsProList[k].push_back(hh2);
          TH2I *hh3 = new TH2I(Form("Jacobi_T_gs_%s_%s%s",iso,reac,beams->sb[k].at(i).c_str()),"",25,0.,1.,20,-1,1); Jacobi_T_List[k].push_back(hh3);
          TH2I *hh4 = new TH2I(Form("Jacobi_Y_gs_%s_%s%s",iso,reac,beams->sb[k].at(i).c_str()),"",25,0.,1.,20,-1,1); Jacobi_Y_List[k].push_back(hh4);
          TH1I *hh5 = new TH1I(Form("ppRel_%s_%s%s",iso,reac,beams->sb[k].at(i).c_str()),"",1800,0,180); ppRelList[k].push_back(hh5);
          TH1I *hh6 = new TH1I(Form("ppRel2_%s_%s%s",iso,reac,beams->sb[k].at(i).c_str()),"",1800,0,180); ppRelList2[k].push_back(hh6);
        }
      }
      dirCorrP[k][j]->cd();
    }
    dirGammas[k] = dirCorrPB[k]->mkdir("gamma");
    dirGammas[k]->cd();
    for(unsigned int j = 0; j < beams->sb[k].size(); j++) {
      TH1I *h = new TH1I(Form("TEC_%s_%s",beams->pb.at(k).c_str(),beams->sb[k].at(j).c_str()),"",800,0,17); TEClist[k].push_back(h);
    }
    dirCorrPB[k]->cd();
  }
  file_read->cd();
  ErelLi6 = new TH1I("ErelLi6","",800,0,17);
  ThetaCMLi6 = new TH1I("ThetaCMLi6","",400,0,20);
  VCMLi6= new TH1I("VCMLi6","",400,0,14);

  ErelBe8 = new TH1I("ErelBe8","",1600,0,17);
  ThetaCMBe8 = new TH1I("ThetaCMBe8","",400,0,20);
  VCMBe8= new TH1I("VCMBe8","",400,0,14);

  ErelB9 = new TH1I("ErelB9","",1600,0,17);
  ErelC12 = new TH1I("ErelC12","",1600,0,34);
  ErelC12Be8 = new TH1I("ErelC12Be8","",1600,0,34);
}
//*********************************************//
void histo_read::write() {
  file_read->Write();
  cout << "histo written" << endl;
  file_read->Close();
}
