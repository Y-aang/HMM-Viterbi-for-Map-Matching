#include <bits/stdc++.h>
#define N 121380
#define pi 3.1415926535897932384626433832795
#define EARTH_RADIUS 6378.137 //地球半径 KM

using namespace std;
typedef double D;
typedef int LL;
ofstream fout;
LL n, m;
struct Road
{
    LL num, type;
    LL start_node, end_node;
    vector<pair<D, D> > node;
}road[N];//路网
vector<LL> next_road[N];
vector<LL> pre_road[N];
LL err[N];//路段是否被选择
vector<LL> chosen_road;

vector<pair<D, D> > trace;//每条实时轨迹的坐标序列
vector<LL> time_node;
vector<pair<D, D> > trace_projection[N];
//D A[2000][2000];//状态转移概率
vector<D> A[N];



D distance(pair<D, D> x, pair<D, D> a, pair<D, D> b )
{
    D tmp1, tmp2;
    tmp1=( b.first - a.first ) * ( x.first - a.first ) + ( b.second - a.second ) * ( x.second - a.second );
    tmp2=( b.first - a.first ) * ( x.first - b.first ) + ( b.second - a.second ) * ( x.second - b.second );
    if( tmp1 * tmp2 < 0 )
        return abs( ( a.second - b.second ) * ( x.first - a.first ) + ( b.first - a.first ) * ( x.second - a.second) ) / hypot( a.second - b.second, b.first - a.first );
    else
        return min( hypot( x.first-a.first, x.second - a.second ), hypot( x.first-b.first, x.second - b.second ) );
}
D road_min_distance(D x, D y, LL num)//计算最短距离
{
    D ans=10;
    for(auto it = road[num].node.begin(); ;)
    {
        D tmp;
        auto it0=it;
        it++;
        if(it == road[num].node.end() )
            break;
        tmp= distance( make_pair(x, y), *it0, *it );
        ans = min(ans, tmp);
    }
    return ans;
}


vector<LL> select_road()
{
    vector<LL> tmp;
    D x = trace[0].first, y = trace[0].second;
    LL ans = 0;
    LL pos=0;
    D min_distance = 10;
    for(LL i=0; i<n; i++)
    {
        D tmp;
        tmp= road_min_distance(x, y, i );
        if( tmp < min_distance )
        {
            min_distance=tmp;
            ans=i;
        }
    }
    memset(err, 0, sizeof(err));
    tmp.push_back(ans);
    err[ans]=1;
//    cout<<"ans="<<ans<<endl;
    while( tmp.size()<800 && pos<tmp.size() )//从起点开始
    {
        if(tmp.size()==1)
        {
             for(LL i=0; i<next_road[road[tmp[pos] ].start_node ].size(); i++)//新增开始点后连接的路段
            {
                if(err[ next_road[road[tmp[pos] ].start_node ][i] ] == 0)
                {
                    tmp.push_back(next_road[road[tmp[pos] ].start_node ][i]);
                    err[ next_road[road[tmp[pos] ].start_node ][i] ] = 1;
                }
            }
        }

        for(LL i=0; i<next_road[road[tmp[pos] ].end_node ].size(); i++)//新增结束点后连接的路段
        {
            if(err[ next_road[road[tmp[pos] ].end_node ][i] ] == 0)
            {
                tmp.push_back(next_road[road[tmp[pos] ].end_node ][i]);
                err[ next_road[road[tmp[pos] ].end_node ][i] ] = 1;
            }
        }
        pos++;
    }


    x = trace[trace.size() - 1].first;
    y = trace[trace.size() - 1].second;
    min_distance = 10;
    for(LL i=0; i<n; i++)
    {
        D tmp;
        tmp= road_min_distance(x, y, i );
        if( tmp < min_distance )
        {
            min_distance=tmp;
            ans=i;
        }
    }
    if(err[ans] == 1)
        return tmp;
    tmp.push_back(ans);
    pos=tmp.size()-1;
    while( tmp.size()<1500 && pos<tmp.size() )//从起点开始
    {
        for(LL i=0; i<pre_road[road[tmp[pos] ].start_node ].size(); i++)//新增开始点后连接的路段
        {
            if(err[ pre_road[road[tmp[pos] ].start_node ][i] ] == 0)
            {
                tmp.push_back(pre_road[road[tmp[pos] ].start_node ][i]);
                err[ pre_road[road[tmp[pos] ].start_node ][i] ] = 1;
            }
        }
        pos++;
    }
    return tmp;
}
//vector<LL> select_road()
//{
//    vector<LL> tmp;
//    D x = trace[0].first, y = trace[0].second;
//    LL ans = 0;
//    LL pos=0;
//    D min_distance = 10;
//    for(LL i=0; i<n; i++)
//    {
//        D tmp;
//        tmp= road_min_distance(x, y, i );
//        if( tmp < min_distance )
//        {
//            min_distance=tmp;
//            ans=i;
//        }
//    }
//    memset(err, 0, sizeof(err));
//    tmp.push_back(ans);
//    err[ans]=1;
////    cout<<"ans="<<ans<<endl;
//    while( tmp.size()<600 && pos<tmp.size() )//从起点开始
//    {
//        if(tmp.size()==1)
//        {
//             for(LL i=0; i<next_road[road[tmp[pos] ].start_node ].size(); i++)//新增开始点后连接的路段
//            {
//                if(err[ next_road[road[tmp[pos] ].start_node ][i] ] == 0)
//                {
//                    tmp.push_back(next_road[road[tmp[pos] ].start_node ][i]);
//                    err[ next_road[road[tmp[pos] ].start_node ][i] ] = 1;
//                }
//            }
//        }
//
//        for(LL i=0; i<next_road[road[tmp[pos] ].end_node ].size(); i++)//新增结束点后连接的路段
//        {
//            if(err[ next_road[road[tmp[pos] ].end_node ][i] ] == 0)
//            {
//                tmp.push_back(next_road[road[tmp[pos] ].end_node ][i]);
//                err[ next_road[road[tmp[pos] ].end_node ][i] ] = 1;
//            }
//        }
//        pos++;
//    }
//
//
//    x = trace[trace.size() - 1].first;
//    y = trace[trace.size() - 1].second;
//    min_distance = 10;
//    for(LL i=0; i<n; i++)
//    {
//        D tmp;
//        tmp= road_min_distance(x, y, i );
//        if( tmp < min_distance )
//        {
//            min_distance=tmp;
//            ans=i;
//        }
//    }
//    if(err[ans] == 1)
//        return tmp;
//    tmp.push_back(ans);
//
//    D ans1=0;
//    min_distance = 10;
//    for(LL i=0; i<n; i++)
//    {
//        if(i == ans)
//            continue;
//        D tmp;
//        tmp= road_min_distance(x, y, i );
//        if( tmp < min_distance )
//        {
//            min_distance=tmp;
//            ans1=i;
//        }
//    }
//    tmp.push_back(ans1);
//
//    D ans2=0;
//    min_distance = 10;
//    for(LL i=0; i<n; i++)
//    {
//        if(i == ans || i== ans1)
//            continue;
//        D tmp;
//        tmp= road_min_distance(x, y, i );
//        if( tmp < min_distance )
//        {
//            min_distance=tmp;
//            ans2=i;
//        }
//    }
//    tmp.push_back(ans2);
//    pos=tmp.size()-3;
//    while( tmp.size()<1600 && pos<tmp.size() )//从起点开始
//    {
//        for(LL i=0; i<pre_road[road[tmp[pos] ].start_node ].size(); i++)//新增开始点后连接的路段
//        {
//            if(err[ pre_road[road[tmp[pos] ].start_node ][i] ] == 0)
//            {
//                tmp.push_back(pre_road[road[tmp[pos] ].start_node ][i]);
//                err[ pre_road[road[tmp[pos] ].start_node ][i] ] = 1;
//            }
//        }
//        pos++;
//    }
//    return tmp;
//}

void get_projection()
{
    for(LL i=0; i<trace.size(); i++)
    {
        vector<pair<D, D> > new_tmp;
        for(LL j=0; j<chosen_road.size(); j++)//考察第i条trace到第chosen_road[j]上的投影
        {
            D ans=10;
            auto closest_node = road[chosen_road[j] ].node.begin();
            for(auto it = road[chosen_road[j] ].node.begin(); ;)
            {
                D tmp;
                auto it0=it;
                it++;
                if(it == road[chosen_road[j] ].node.end() )
                    break;
                tmp= distance( make_pair(trace[i].first, trace[i].second), *it0, *it );
                if(tmp < ans)
                {
                    ans=tmp;
                    closest_node = it0;
                }
            }
            pair<D, D> a((*closest_node).first, (*closest_node).second);
            closest_node++;
            pair<D, D> b((*closest_node ).first, (*closest_node).second );
            pair<D, D> x(trace[i].first, trace[i].second);
            D tmp1, tmp2;
            tmp1=( b.first - a.first ) * ( x.first - a.first ) + ( b.second - a.second ) * ( x.second - a.second );
            tmp2=( b.first - a.first ) * ( x.first - b.first ) + ( b.second - a.second ) * ( x.second - b.second );
            if( tmp1 * tmp2 < 0 )
            {
                D delta_x1, delta_x2;
                delta_x1 = ( ( b.first - a.first ) * ( x.first - a.first ) + ( b.second - a.second ) * ( x.second - a.second ) )
                            / hypot( a.first - b.first, a.second - b.second ) / hypot( a.first - b.first, a.second - b.second ) * ( b.first - a.first );
                delta_x2 = ( ( b.first - a.first ) * ( x.first - a.first ) + ( b.second - a.second ) * ( x.second - a.second ) )
                            / hypot( a.first - b.first, a.second - b.second ) / hypot( a.first - b.first, a.second - b.second ) * ( b.second - a.second );
                new_tmp.push_back(make_pair(a.first + delta_x1, a.second + delta_x2) );
            }
            else
            {
                if(hypot( x.first-a.first, x.second - a.second ) < hypot( x.first-b.first, x.second - b.second ))
                    new_tmp.push_back(a);
                else
                    new_tmp.push_back(b);
            }
        }
        trace_projection[i] = new_tmp;
    }
}




void calculate_A()
{
    for(LL i=0; i<chosen_road.size(); i++)//考察第i条路的转移概率
    {
        A[i].clear();
        vector<D> tmp;
        vector<LL> memory[15];
        for(LL j=0; j<chosen_road.size(); j++)
        {
            if(road[chosen_road[i] ].end_node == road[chosen_road[j] ].start_node && road[chosen_road[i] ].start_node == road[chosen_road[j] ].end_node )
                tmp.push_back(0.7);
            else if( road[chosen_road[i] ].end_node == road[chosen_road[j] ].start_node || i == j)
            {
                tmp.push_back(1);
                if(i!=j)
                    memory[0].push_back(j);
            }
            else
                tmp.push_back(0);
        }
        D possibility=1;
        for(LL kk=0; kk<3  ; kk++)
        {
            if(kk==0)
                possibility=1;
            else if(kk ==1)
                possibility=0.97;
            else
                possibility=0.5;
            for(LL k=0; k<memory[kk].size(); k++)
            {
                for(LL j=0; j<chosen_road.size(); j++)
                {
                    if(tmp[j]==0 && road[chosen_road[memory[kk][k] ] ].end_node == road[chosen_road[j] ].start_node && road[chosen_road[memory[kk][k] ] ].start_node == road[chosen_road[j] ].end_node )
                        tmp[j]=0.7;
                    if(tmp[j]==0 && road[chosen_road[memory[kk][k] ] ].end_node == road[chosen_road[j] ].start_node )
                    {
                        tmp[j]=possibility;
                        memory[kk+1].push_back(j);
                    }
                }
            }
        }
        for(LL k=0; k<tmp.size(); k++)
            if(tmp[k]==0)
                tmp[k]=0.01;


        A[i]=tmp;
    }
}

D calculate_sigma(vector<D> dst)
{
    LL siz = dst.size();
    siz /= 2;
    nth_element(dst.begin(), dst.begin() + siz, dst.end() );
    return dst[siz] * 1.4826;
}


double rad(double d)
{
    return d * pi /180.0;
}
double RealDistance(double lat1,double lng1,double lat2,double lng2)//lat1第一个点纬度,lng1第一个点经度,lat2第二个点纬度,lng2第二个点经度
{

	double a;
   	double b;
   	double radLat1 = rad(lat1);
   double radLat2 = rad(lat2);
   a = radLat1 - radLat2;
   b = rad(lng1) - rad(lng2);
   double s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
    s = s * EARTH_RADIUS;
    s = s * 1000;
    return s;
}

D highest_speed(LL p)
{
    LL type;
    type = road[p].type;
    switch(type)
    {
    case 1:
        return 5;
    case 2:
        return 10;
    case 3:
        return 10;
    case 4:
        return 20;
    case 5:
        return 30;
    case 6:
        return 40;
    case 7:
        return 120;
    }
    return 120;
}

D calculate_angle(LL p, LL q)//第p条trace，第q条真路
{
    if(p==0)
        return 1;
    D ans=0;
    pair<D, D> m(trace[p].first-trace[p-1].first,trace[p].second-trace[p-1].second );
    for(LL i=0; i<road[q ].node.size()-1; i++)
    {
        pair<D, D> tmp(road[q].node[i+1].first-road[q].node[i].first, road[q ].node[i+1].second-road[q ].node[i].second);
        ans= max(ans, abs (    (m.first* tmp.first + m.second * tmp.second )/ hypot(m.first, m.second) / hypot(tmp.first, tmp.second)     )  );
    }
    return ans;
}

vector<D> calculate_emission_possibility(D x, D y, LL p)
{
    vector<D> tmp;
    vector<D> dst;
    for(LL i=0; i<chosen_road.size(); i++)
        dst.push_back( road_min_distance(x, y, chosen_road[i]) );
//    for(LL i=0; i<chosen_road.size(); i++)
//        dst[i] *= (1 - 0.8 *  calculate_angle(p, chosen_road[i]) ) ;
    D sigma;
    sigma = calculate_sigma(dst);
    for(LL i=0; i<chosen_road.size(); i++)
    {
        D ans;
        ans = 1. / ( sigma * sqrt( 2 * M_PI ) ) * exp( -0.5 * dst[i] / sigma * dst[i] / sigma );
        if(p !=trace.size()-1)
        {
            D s, v;
            s=RealDistance(trace_projection[p][i].first, trace_projection[p][i].second, trace_projection[p+1][i].first, trace_projection[p+1][i].second);
            v=s/( time_node[p+1]- time_node[p]) * 3.6;
//            cout<<s<<' '<<v<<endl;
            if(v>highest_speed(chosen_road[p]))
            {
                ans *= 0.6;
            }

        }
//        ans= ans* (0.3 + 0.7 * calculate_angle(p, chosen_road[i]) );

        tmp.push_back( ans );
    }
    return tmp;
}




void Viterbi()
{
    LL siz=trace.size();
    vector< pair<D, LL> > V[siz+1];
    LL ans[ siz + 1 ];
    for(LL i=0; i<chosen_road.size(); i++)
    {
        V[0].push_back( make_pair(1, -1) );
    }
//    cout<<"siz="<<siz<<endl;
    for(LL i=1; i<=siz; i++)//考察到trace[i-1]、从V[1]开始算trace[0]
    {
        vector<D> emission_possibility;
//        cout<<i<<endl;
        emission_possibility = calculate_emission_possibility(trace[i-1].first, trace[i-1].second, i-1);

        for(LL j=0; j<chosen_road.size(); j++)//到第chosen_road[j]条路的概率
        {
            D possibility = V[i-1][0].first * A[0][j] * emission_possibility[j];
            LL number = 0;
            for(LL k=1; k<chosen_road.size(); k++)
            {
                D tmp = V[i-1][k].first * A[k][j] * emission_possibility[j];
                if(tmp > possibility)
                {
                    possibility = tmp;
                    number = k;
                }
            }
            V[i].push_back( make_pair(possibility, number) );
        }
    }



    D biggest_possibility = V[siz][0].first;
    D biggest_number = 0;
    for(LL i=1; i<chosen_road.size(); i++)//寻找最大最终概率
    {
        if( V[siz][i].first > biggest_possibility)
        {
            biggest_possibility = V[siz][i].first;
            biggest_number = i;
        }
    }
    //放置数组a
    LL b[siz+1];
    b[siz] = biggest_number;
    for(LL i = siz-1; i>=1; i--)
    {
        b[i] = V[i+1][ b[i+1] ].second ;
    }
//    for(LL i=1; i<=siz; i++)
//        printf("%d ", chosen_road[b[i] ]);
//    printf("\n");
    for(LL i=1; i<=siz; i++)
        fout<<chosen_road[b[i] ] <<' ';
    fout<<'\n';
}



int main()
{
    ifstream myfile;
    myfile.open("sample.in");
    fout.open("my out.txt");
    if (myfile.is_open())
	{
		cout << "can open this file" << endl;
	}
    myfile>>n;
//    scanf("%d", &n);

//读入路网
    for(LL i=0; i<n; i++)
    {
        LL t;
        D x, y;
        string s;
        myfile>>road[i].num;
        myfile>>road[i].start_node;
        myfile>>road[i].end_node;
        myfile>>s;
        myfile>>t;
        myfile>>t;
//        scanf("%d", &road[i].num);
//        scanf("%d", &road[i].start_node);
//        scanf("%d", &road[i].end_node);
//        cin>>s;
//        scanf("%d", &t);
//        scanf("%d", &t);
        for(LL j=0; j<t; j++)
        {
//            scanf("%lf", &x);
//            scanf("%lf", &y);
            myfile>>x;
            myfile>>y;
            road[i].node.push_back( make_pair(x, y) );
        }
    }


    for(LL i=0; i<n; i++)//构建next_road
    {
        next_road[road[i].start_node ].push_back(i);
    }
    for(LL i=0; i<n; i++)//构建next_road
    {
        pre_road[road[i].end_node ].push_back(i);
    }






//读入路线+查找
//    scanf("%d", &m);
    myfile>>m;
    printf("%d\n", m);
    for(LL i=0; i<m; i++)
    {
        LL t;
        D x, y;
//        scanf("%d", &t);
        myfile>>t;
        trace.clear();
        time_node.clear();
        do      //读入路径
        {
//            scanf("%lf", &x);
//            scanf("%lf", &y);
            myfile>>x>>y;
            time_node.push_back(t);
            trace.push_back( make_pair(x, y) );
//            scanf("%d", &t);
            myfile>>t;
        }while(t != i);
        chosen_road.clear();
        chosen_road = select_road();

        calculate_A();
        get_projection();

        Viterbi();     //计算结果
        cout<<"i="<<i<<endl;
    }
    fout.close();
    return 0;
}
