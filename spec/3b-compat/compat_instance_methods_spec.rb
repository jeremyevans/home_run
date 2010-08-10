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

  it "#gregorian, #england, #julian, #italy, and #new_start should be the same as self" do
    @d.gregorian.should == @d
    @d.england.should == @d
    @d.julian.should == @d
    @d.italy.should == @d
    @d.new_start.should == @d
    @d.new_start(1).should == @d
  end

  it "#gregorian? and ns? should be true" do
    @d.gregorian?.should == true
  end

  it "#julian? and os? should be false" do
    @d.julian?.should == false
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
  end

  ruby_version_is "" ... "1.9" do
    it "#newsg should be the same as self" do
      @d.newsg.should == @d
      @d.newsg(1).should == @d
    end

    it "ns? should be true" do
      @d.ns?.should == true
    end

    it "os? should be false" do
      @d.os?.should == false
    end

    it "#sg should be less than a low number" do
      @d.sg.should < -(2**30)
    end
  end

end
