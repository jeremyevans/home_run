require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#+" do

  it "should add a number of days to a Date" do
    (DateTime.civil(2007,2,27) + 315).should == DateTime.civil(2008, 1, 8)
    (DateTime.commercial(2007,2,2) + 315).should == DateTime.commercial(2007, 47, 2)
    (DateTime.jd(2454782) + 315).should == DateTime.jd(2455097)
    (DateTime.ordinal(2008, 10) + 315).should == DateTime.ordinal(2008, 325)
  end
  
  it "should add a fractional number of days to a Date" do
    (DateTime.civil(2007,2,27) + 315.5).should == DateTime.civil(2008, 1, 8, 12)
    (DateTime.commercial(2007,2,2) + 315.75).should == DateTime.commercial(2007, 47, 2, 18)
    (DateTime.jd(2454782) + 315.25).should == DateTime.jd(2455097, 6)
    (DateTime.ordinal(2008, 10) + 315.25).should == DateTime.ordinal(2008, 325, 6)
  end
  
  it "should add a negative number of days to a Date" do
    d = DateTime.civil(2007,2,27).+(-10)
    d.should == DateTime.civil(2007, 2, 17)
  end
  
  it "should add a fractional negative number of days to a Date" do
    d = DateTime.civil(2007,2,27).+(-10.5)
    d.should == DateTime.civil(2007, 2, 16, 12)
  end

  it "should keep the same class as the receiver" do
    c = Class.new(DateTime)
    c.jd.+(10).should be_kind_of(c)
  end

  it "should raise an error on non numeric parameters" do
    lambda { DateTime.civil(2007,2,27) + :hello }.should raise_error
    lambda { DateTime.civil(2007,2,27) + "hello" }.should raise_error
    lambda { DateTime.civil(2007,2,27) + DateTime.new(2007,2,27) }.should raise_error
    lambda { DateTime.civil(2007,2,27) + Object.new }.should raise_error
  end
  
end
