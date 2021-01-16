#include "core.hpp"

#include <opencv2/opencv.hpp>

#include <catch2/catch.hpp>

const std::string resource_path = "/home/bvi/projects/novavision/resources/";

TEST_CASE("run core.hpp tests", "[core]")
{
    REQUIRE(true);
}

TEST_CASE("nv::sequence constructors.", "[core]")
{
    nv::sequence sequence1;
    REQUIRE(sequence1.size() == 0);
    nv::sequence sequence2(124);
    REQUIRE(sequence2.size() == 0);
}

TEST_CASE("nv::sequence initialize list.", "[core]")
{
    nv::sequence sequence{{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
    REQUIRE(sequence.size() == 6);
    REQUIRE(sequence.at(2) == cv::Point(2, 2));
}

TEST_CASE("nv::sequence push and iterators.", "[core]")
{
    std::vector<cv::Point> vec{{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
    nv::sequence sequence(vec.size());

    for(auto const& v: vec)
    {
        sequence.push(v);
    }
    size_t i = 0;
    for(auto const& p: sequence)
    {
        REQUIRE(p == vec[i++]);
    }
}


TEST_CASE("nv::descriptor", "[core]")
{
    auto lm = cv::imread(resource_path + "moto/im0.png", 0);
    REQUIRE(!lm.empty());
    auto rm = lm.clone();//cv::imread("../resources/moto/im1.png");
    REQUIRE(!rm.empty());

    nv::sequence sequence{{0,0}, {10,10}, {10,-10}, {-10,10}, {-10, -10}, {25,25}, {25,-25}, {-25,25}, {-25, -25}};
    nv::descriptor ld(lm, {1464, 1333}, sequence);
    nv::descriptor rd(rm, {1464, 1333}, ld.permitted_sequence());//{1260, 1333}

    REQUIRE(ld.size() == sequence.size());
    REQUIRE(ld.size() == rd.size());

    for(size_t i = 0; i < ld.size(); ++i)
    {
        REQUIRE(ld.at(i) == rd.at(i));
    }
}

TEST_CASE("nv::compare", "[core]")
{
    auto lm = cv::imread(resource_path + "moto/im0.png", 0);
    REQUIRE(!lm.empty());
    auto rm = cv::imread(resource_path + "moto/im1.png", 0);
    REQUIRE(!rm.empty());

    cv::Point lp{1464, 1333};
    cv::Point rp{1264, 1333};

    nv::sequence sequence{{0,0}, {10,10}, {10,-10}, {-10,10}, {-10, -10}, {25,25}, {25,-25}, {-25,25}, {-25, -25}};
    nv::descriptor ld(lm, lp, sequence);
    nv::descriptor rd(rm, rp, ld.permitted_sequence());

    REQUIRE(ld.size() == sequence.size());
    REQUIRE(ld.size() == rd.size());

    auto metric = nv::correlation_compare(ld, rd, 0.9);

    //std::cout << "COEFF: " << metric.coeff() << std::endl;

    REQUIRE(static_cast<bool>(metric));

    REQUIRE(metric.coeff() > 0.969);
}


TEST_CASE("nv::fibonacci_sequence", "[core]")
{
    auto lm = cv::imread(resource_path + "moto/im0.png", 0);
    REQUIRE(!lm.empty());
    auto rm = cv::imread(resource_path + "moto/im1.png", 0);
    REQUIRE(!rm.empty());

    cv::Point lp{1464, 1333};
    cv::Point rp{1264, 1333};

    nv::fibonacci_sequence sequence{32, 50, 0.5};
    nv::descriptor ld(lm, lp, sequence);
    nv::descriptor rd(rm, rp, ld.permitted_sequence());

    REQUIRE(ld.size() == sequence.size());
    REQUIRE(ld.size() == rd.size());

    auto metric = nv::correlation_compare(ld, rd, 0.9);

    //std::cout << "COEFF: " << metric.coeff() << std::endl;

    REQUIRE(static_cast<bool>(metric));

    REQUIRE(metric.coeff() > 0.927);
}

TEST_CASE("nv::find_matching_point", "[core]")
{
    auto lm = cv::imread(resource_path + "moto/im0.png", 0);
    REQUIRE(!lm.empty());
    auto rm = cv::imread(resource_path + "moto/im1.png", 0);
    REQUIRE(!rm.empty());

    cv::Point lp{1464, 1333};
    cv::Point rp = nv::find_matching_point(lm, rm, lp);

    REQUIRE(rp == cv::Point{1263, 1333});

    // cv::circle(lm, lp, 2, 0, 1);
    // cv::circle(rm, rp, 2, 0, 1);

    // cv::namedWindow("RIGHT", cv::WINDOW_NORMAL);
    // cv::namedWindow("LEFT", cv::WINDOW_NORMAL);

    // cv::imshow("RIGHT", rm);
    // cv::imshow("LEFT", lm);

    // cv::waitKey(0);
}


/*
TEST_CASE("nv::depth map", "[core]")
{
    auto lm = cv::imread(resource_path + "moto/im0.png", 0);
    REQUIRE(!lm.empty());
    auto rm = cv::imread(resource_path + "moto/im1.png", 0);
    REQUIRE(!rm.empty());

    auto depth_map = nv::build_depth_map_multy_line(lm, rm);

    cv::namedWindow("DEPTH", cv::WINDOW_NORMAL);
    cv::imshow("DEPTH", depth_map);

    cv::waitKey(0);

    // cv::circle(lm, lp, 2, 0, 1);
    // cv::circle(rm, rp, 2, 0, 1);

    // cv::namedWindow("RIGHT", cv::WINDOW_NORMAL);
    // cv::namedWindow("LEFT", cv::WINDOW_NORMAL);

    // cv::imshow("RIGHT", rm);
    // cv::imshow("LEFT", lm);

    // cv::waitKey(0);
}
*/