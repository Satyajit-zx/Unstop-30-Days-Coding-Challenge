#include <stdio.h>
#include <stdlib.h>
typedef long long ll;

int cmp(const void*a,const void*b){
    ll x=*(ll*)a,y=*(ll*)b;
    return (x>y)-(x<y);
}

int lb(ll*a,int n,ll x){
    int l=0,r=n;
    while(l<r){
        int m=(l+r)/2;
        if(a[m]<x) l=m+1;
        else r=m;
    }
    return l;
}

int main(){
    int n; ll D;
    scanf("%d%lld",&n,&D);

    ll *a=malloc(n*sizeof(ll)),*b=malloc(n*sizeof(ll));
    for(int i=0;i<n;i++) scanf("%lld",&a[i]),b[i]=a[i];

    qsort(b,n,sizeof(ll),cmp);

    int m=0;
    for(int i=0;i<n;i++)
        if(!i || b[i]!=b[i-1]) b[m++]=b[i];

    int *last=malloc(m*sizeof(int));
    int *mn=malloc(n*sizeof(int));
    int *mx=malloc(n*sizeof(int));

    for(int i=0;i<m;i++) last[i]=-1;

    int mh=0,mt=0,xh=0,xt=0;
    int l=0,best=0,start=0;

    for(int r=0;r<n;r++){
        int id=lb(b,m,a[r]);

        if(last[id]>=l) l=last[id]+1;
        last[id]=r;

        while(mh<mt && a[mn[mt-1]]>=a[r]) mt--;
        mn[mt++]=r;

        while(xh<xt && a[mx[xt-1]]<=a[r]) xt--;
        mx[xt++]=r;

        /* Remove elements that are outside the window */
        while(mh<mt && mn[mh]<l) mh++;
        while(xh<xt && mx[xh]<l) xh++;

        /* Fix max-min constraint */
        while(a[mx[xh]]-a[mn[mh]]>D){
            l++;

            while(mh<mt && mn[mh]<l) mh++;
            while(xh<xt && mx[xh]<l) xh++;
        }

        if(r-l+1>best){
            best=r-l+1;
            start=l;
        }
    }

    printf("%d %d\n",best,start+1);

    free(a);
    free(b);
    free(last);
    free(mn);
    free(mx);

    return 0;
}
