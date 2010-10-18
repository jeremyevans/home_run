require File.expand_path('../../spec_helper', __FILE__)

describe "Date#+" do

  it "should add a number of days to a Date" do
    (Date.civil(2007,2,27) + 315).should == Date.civil(2008, 1, 8)
    (Date.commercial(2007,2,2) + 315).should == Date.commercial(2007, 47, 2)
    (Date.jd(2454782) + 315).should == Date.jd(2455097)
    (Date.ordinal(2008, 10) + 315).should == Date.ordinal(2008, 325)
  end
  
  it "should add a negative number of days to a Date" do
    d = Date.civil(2007,2,27).+(-10)
    d.should == Date.civil(2007, 2, 17)
  end

  it "should keep the same class as the receiver" do
    c = Class.new(Date)
    c.jd.+(10).should be_kind_of(c)
  end

  it "should raise an error on non numeric parameters" do
    lambda { Date.civil(2007,2,27) + "hello" }.should raise_error(TypeError)
    lambda { Date.civil(2007,2,27) + Date.new }.should raise_error(TypeError)
    lambda { Date.civil(2007,2,27) + Object.new }.should raise_error(TypeError)
  end
  
end
