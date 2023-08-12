//
// Created by Nathan Ellis on 07/04/2023.
//

#ifndef CLIENT_CITYCOORDINATES_H
#define CLIENT_CITYCOORDINATES_H
#include <iostream>
#include <string>
#include <map>

using namespace std;

/**
 * @brief A map containing the latitude and longitude coordinates of all cities supported by the system.
 *
 * This map contains the latitude and longitude coordinates of various cities in the United Kingdom.
 * Each city is represented by a string key, and the corresponding value is a pair of double values
 * representing the latitude and longitude coordinates, respectively.
 */
map<string, pair<double, double>> cityCoordinates = {
        {"Aberdeen",            {57.1497,   -2.0943}},
        {"Armagh",              {54.3503,   -6.6528}},
        {"Bangor",              {53.2274,   -4.1269}},
        {"Bath",                {51.3811,   -2.3590}},
        {"Belfast",             {54.5973,   -5.9301}},
        {"Birmingham",          {52.4800,   -1.9100}},
        {"Bradford",            {53.7950,   -1.7594}},
        {"Brighton",            {50.8225,   -0.1372}},
        {"Bristol",             {51.4545,   -2.5879}},
        {"Cambridge",           {52.2053,   0.1218}},
        {"Canterbury",          {51.2802,   1.0789}},
        {"Cardiff",             {51.4816,   -3.1791}},
        {"Carlisle",            {54.8925,   -2.9329}},
        {"Chelmsford",          {51.7356,   0.4685}},
        {"Chester",             {53.1934,   -2.8931}},
        {"Chichester",          {50.8376,   -0.7740}},
        {"Coventry",            {52.4068,   -1.5197}},
        {"Derby",               {52.9225,   -1.4746}},
        {"Derry",               {54.9966,   -7.3086}},
        {"Dundee",              {56.4620,   -2.9707}},
        {"Durham",              {54.7753,   -1.5849}},
        {"Edinburgh",           {55.9533,   -3.1883}},
        {"Ely",                 {52.3970,   0.2660}},
        {"Exeter",              {50.7184,   -3.5339}},
        {"Glasgow",             {55.8652,   -4.2576}},
        {"Gloucester",          {51.8642,   -2.2448}},
        {"Hereford",            {52.0565,   -2.7160}},
        {"Inverness",           {57.4778,   -4.2247}},
        {"Kingston upon Hull",  {53.7676,   -0.3274}},
        {"Lancaster",           {54.0470,   -2.8010}},
        {"Leeds",               {53.8008,   -1.5491}},
        {"Leicester",           {52.6369,   -1.1398}},
        {"Lichfield",           {52.6832,   -1.8262}},
        {"Lincoln",             {53.2307,   -0.5406}},
        {"Lisburn",             {54.5162,   -6.0580}},
        {"Liverpool",           {53.4084,   -2.9916}},
        {"London",              {51.507222, -0.127500}},
        {"Manchester",          {53.4800,   -2.2400}},
        {"Newcastle upon Tyne", {54.9714,   -1.6174}},
        {"Newport",             {51.5882,   -2.9977}},
        {"Newry",               {54.1753,   -6.340}},
        {"Newport",             {51.5882,   -2.9977}},
        {"Newry",               {54.1753,   -6.3402}},
        {"Norwich",             {52.6309,   1.2974}},
        {"Nottingham",          {52.9548,   -1.1581}},
        {"Oxford",              {51.7520,   -1.2577}},
        {"Peterborough",        {52.5695,   -0.2405}},
        {"Plymouth",            {50.3755,   -4.1427}},
        {"Portsmouth",          {50.8198,   -1.0873}},
        {"Preston",             {53.7632,   -2.7031}},
        {"Ripon",               {54.1381,   -1.5241}},
        {"Salford",             {53.4875,   -2.2901}},
        {"Salisbury",           {51.0688,   -1.7945}},
        {"Sheffield",           {53.3811,   -1.4701}},
        {"Southampton",         {50.9097,   -1.4044}},
        {"St Albans",           {51.7510,   -0.3363}},
        {"St Asaph",            {53.2575,   -3.4418}},
        {"St Davids",           {51.8812,   -5.2660}},
        {"Stirling",            {56.1165,   -3.9369}},
        {"Stoke-on-Trent",      {53.0027,   -2.1794}},
        {"Sunderland",          {54.9047,   -1.3815}},
        {"Swansea",             {51.6214,   -3.9436}},
        {"Truro",               {50.2632,   -5.0510}},
        {"Wakefield",           {53.6833,   -1.4977}},
        {"Wells",               {51.2090,   -2.6476}},
        {"Westminster",         {51.4995,   -0.1357}},
        {"Winchester",          {51.0632,   -1.3080}},
        {"Wolverhampton",       {52.5883,   -2.1291}},
        {"Worcester",           {52.1920,   -2.2217}},
        {"York",                {53.9591,   -1.0819}}
};
#endif //CLIENT_CITYCOORDINATES_H
