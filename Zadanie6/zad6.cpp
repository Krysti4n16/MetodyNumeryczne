#include<iostream>
#include<vector>
#include<complex>
#include<cmath>
#include<iomanip>

using Complex=std::complex<double>;
using Poly=std::vector<Complex>;

const double EPSILON= 1e-14;

Complex evalPoly(const Poly&P,Complex x){
    Complex result=P[P.size()-1];
    for(int i=P.size()-2;i>=0;i--){
        result= result*x+P[i];
    }
    return result;
}

Poly derivative(const Poly&P){
    if(P.size()<=1){
        Poly Z;
        Z.push_back(0);
        return Z;
    }
    Poly dP;
    for(int i=1;i<P.size();i++){
        dP.push_back(P[i]*(double)i);
    }
    return dP;
}

Complex laguerreOneRoot(const Poly&P,Complex guess){
    int n= P.size()-1;
    Complex x= guess;
    Poly dP= derivative(P);
    Poly d2P= derivative(dP);

    for(int k=0;k<100;k++){
        Complex px= evalPoly(P,x);
        if(std::abs(px)<EPSILON)return x;

        Complex G=evalPoly(dP,x)/px;
        Complex H=G*G-evalPoly(d2P,x)/px;
        Complex rad=std::sqrt((double)(n-1)*((double)n*H-G*G));

        Complex den1=G+rad;
        Complex den2=G-rad;
        Complex den=(std::abs(den1)>std::abs(den2))?den1:den2;

        if(std::abs(den)<EPSILON)break;

        Complex a= (double)n/den;
        x= x-a;

        if(std::abs(a)<EPSILON)return x;
    }
    return x;
}

Poly deflate(const Poly&P,Complex root){
    Poly Q;
    Q.resize(P.size()-1);

    Q[P.size()-2]=P[P.size()-1];
    for(int i=P.size()-3;i>=0;i--){
        Q[i]=P[i+1]+Q[i+1]*root;
    }
    return Q;
}

int main(){
    Poly P= {16.0,-72.0,-28.0,558.0,-990.0,783.0,-486.0,243.0};
    Poly currentP= P;
    std::vector<Complex> roots;

    int degree=P.size()-1;
    for(int i=0;i<degree;i++){
        Complex root=laguerreOneRoot(currentP,Complex(0.0,0.0));
        if(std::abs(root.imag())<1e-8)root.imag(0.0);
        roots.push_back(root);
        currentP=deflate(currentP,root);
    }

    for(int i=0;i<roots.size()-1;i++){
        for(int j=0;j<roots.size()-i-1;j++){
            if(roots[j].real()>roots[j+1].real()){
                Complex tmp= roots[j];
                roots[j]= roots[j+1];
                roots[j+1]= tmp;
            }
        }
    }

    std::cout<<std::fixed<<std::setprecision(12);
    for(int i=0;i<roots.size();i++){
        std::cout<<"x"<<i+1<<"="<<roots[i].real();
        if(std::abs(roots[i].imag())>1e-8){
            if(roots[i].imag()>0)std::cout<<"+";
            std::cout<<roots[i].imag()<<"i";
        }
        std::cout<<"\n";
    }
    return 0;
}
