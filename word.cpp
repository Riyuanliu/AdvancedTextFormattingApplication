//
//  word1.cpp
//  project2b
//
//  Created by Riyuan Liu on 10/31/22.
//



#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

bool header(string line){
    if(line.length()<30 && isupper(line[0]) && isupper(line[1])){
        return true;
    }
    else{
        return false;
    }
}



bool empty_line(string line){
    if(line.length()==0|| !isalpha(line[0]) || line.empty() || line[0]=='\n'){
        return true;
    }
    else{
        return false;
    }
}



string amount_space(int numb){
    string space="";
    for(int i = 0; i < numb-1 ;i++){
        space+=" ";
    }
    return space;
}



string amount_space_center(int length, int max_width){
    int numb;
    string space;
    if(length%2!=0){
        numb = (max_width - length)/2+1;
    }
    else{
        numb = (max_width - length)/2;
    }
    for(int i = 0; i < numb-1 ;i++){
        space+=" ";
    }
    return space;
}



string out_tempt_str(string temp_str,string dir,int max_width){
    string sentence;
    double cur_width=temp_str.length();
    if(dir=="left"){
        sentence = (temp_str+amount_space(max_width - cur_width));
    }
    if(dir=="center"){
        sentence = (amount_space_center(cur_width,max_width)+temp_str+amount_space_center(cur_width,max_width));
    }
    if(dir=="right"){
        sentence = (amount_space(max_width - cur_width)+ temp_str);
    }
    return sentence;
}



int main(){
    string word,firstline,line,title,temp_str="",junk,header_dir,body_dir;
    string outfile_name,infile_name;
    bool to_fill=false;
    bool double_space=false;
    int cur_width=0,max_width=0;
    cout<<"Enter the input filename: ";
    cin>> infile_name;
    int x=0,y=0,z=0,temp_numb;
    
    ifstream infile;
    infile.open(infile_name);
    getline(infile,firstline);
    string temp_word;
    int sub1=0;
    for(int i=0;i<firstline.length();i++){
        if(firstline[i]!=';'){
            temp_word+=firstline[i];
        }
        else if(sub1==0&&firstline[i]==';'){
            max_width = stoi(temp_word);
            temp_word="";
            sub1++;
        }
        else if(sub1==1&&firstline[i]==';'){
            body_dir=temp_word;
            temp_word="";
            sub1++;
        }
        else if(sub1==2&&firstline[i]==';'){
            header_dir=temp_word;
            temp_word="";
            sub1++;
        }
        else if(sub1==3&&firstline[i]==';'){
            if(temp_word=="true"){
                to_fill=true;
            }
            temp_word="";
            sub1++;
        }
        else if(sub1==4&&firstline[i]==';'){
            if(temp_word=="true"){
                double_space=true;
            }
            temp_word="";
            sub1++;
        }
        else if(sub1==5&&firstline[i]==';'){
            outfile_name=temp_word;
            temp_word="";
            sub1++;
        }
    }
    if(body_dir.empty()){
        body_dir = "left";
    }
    if(header_dir.empty()){
        header_dir= body_dir;
    }
    
    ofstream outfile;
    outfile.open(outfile_name);
    while(getline(infile,line)){
        if((empty_line(line)|| header(line))&&x!=0){                        //if nextline is empty or a header, print the rest of temp_str
            outfile << out_tempt_str(temp_str,body_dir,max_width);
            //outfile<<endl<<"0"<<endl;
            if(double_space){
                outfile<<'\n'<<'\n';
            }
            else{
                outfile<<'\n';
            }
            x=0;
            temp_str="";
        }
        if(empty_line(line)){                                                    //if equal to empty line, print \n
            outfile<<'\n';
            cur_width=0;
        }
        else if(header(line)){                                                   //if line is header, print the header
            if(double_space){
                outfile<<'\n';
            }
            outfile << out_tempt_str(line,header_dir,max_width);
            if(double_space){
                outfile<<'\n'<<'\n';
            }
            else{
                outfile<<'\n';
            }
            cur_width=0;
            temp_str="";
        }
        else {
            temp_str+="";
            istringstream sentence(line);
            while(sentence>>word){
                z=0;
                if(cur_width==0){
                    temp_str+=word;
                    cur_width+=word.length();
                }
                else{
                    if(cur_width+word.length()<max_width-1){
                        temp_str+=" "+word;
                        cur_width+=word.length()+1;
                    }
                    else{
                        if(to_fill){
                            if(cur_width<max_width-3){
                                temp_str+=" ";
                                cur_width+=1;
                                for(int i=0;i<word.length();i++){
                                    if(cur_width==max_width-2){
                                        if(cur_width!=max_width-3){
                                            temp_str+="-";
                                        }
    
                                        outfile<<out_tempt_str(temp_str, body_dir, max_width);
                                        if(double_space){
                                            outfile<<'\n'<<'\n';
                                        }
                                        else{
                                            outfile<<'\n';
                                        }
                                        temp_str=word[i];
                                        cur_width=1;
                                    }
                                    else{
                                        temp_str+=word[i];
                                        cur_width+=1;

                                    }
                                }
                            }
                            else{
                                outfile << out_tempt_str(temp_str,body_dir,max_width);
                                //outfile<<endl<<"5"<<endl;
                                if(double_space){
                                    outfile<<'\n'<<'\n';
                                }
                                else{
                                    outfile<<'\n';
                                }
                                temp_str=word;
                                cur_width=word.length();
                            }
                        }
                        else{
                            outfile << out_tempt_str(temp_str,body_dir,max_width);
                            //outfile<<endl<<"5"<<endl;
                            if(double_space){
                                outfile<<'\n'<<'\n';
                            }
                            else{
                                outfile<<'\n';
                            }
                            temp_str="";
                            cur_width=0;
                            temp_str+=word;
                            cur_width+=word.length();
                        }
                    }
                }
                x++;
            }
        }
        y++;
    }
    
    if((!header(line)||!empty_line(line))&&!temp_str.empty()){
        outfile << out_tempt_str(temp_str,body_dir,max_width);
    }
    infile.close();
    outfile.close();
    return 0;
}
