require File.expand_path('../../spec_helper', __FILE__)

describe "Date/DateTime instance methods" do
  before do
    @d = Date.civil(2008, 10, 11)
  end

  it "#ajd should be the same as jd" do
    @d.ajd.should == @d.jd
  end

  it "#amjd should be the astronomical modified julian date" do
    @d.amjd.should == 54750
  end

  it "#gregorian, #england, #julian, #italy, #new_start, and #newsg should be the same as self" do
    @d.gregorian.should == @d
    @d.england.should == @d
    @d.julian.should == @d
    @d.italy.should == @d
    @d.new_start.should == @d
    @d.newsg.should == @d
    @d.new_start(1).should == @d
    @d.newsg(1).should == @d
  end

  it "#gregorian? and ns? should be true" do
    @d.gregorian?.should == true
    @d.ns?.should == true
  end

  it "#julian? and os? should be false" do
    @d.julian?.should == false
    @d.os?.should == false
  end

  it "#ld should be the days since italian calendar reform day" do
    @d.ld.should == 155591
  end

  it "#mday should be the same as day" do
    @d.mday.should == @d.day
  end

  it "#mjd should be the modified julian date" do
    @d.mjd.should == 54750
  end

  it "#start and #sg should be less than a low number" do
    @d.start.should < -(2**30)
    @d.sg.should < -(2**30)
  end

end
