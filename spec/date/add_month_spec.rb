require File.expand_path('../../spec_helper', __FILE__)

describe "Date#>>" do

  it "should add a number of months to a Date" do
    (Date.civil(2007,2,27) >> 10).should == Date.civil(2007, 12, 27)
    (Date.civil(2007,2,27) >> 10).should == Date.civil(2007, 12, 27)
    (Date.commercial(2007,2,2) >> 10).should == Date.commercial(2007, 45, 5)
    (Date.jd(2454782) >> 10).should == Date.jd(2455086)
    (Date.ordinal(2008, 10) >> 10).should == Date.ordinal(2008, 315)
    (Date.civil(2007,2,27) >> 22).should == Date.civil(2008, 12, 27)
    (Date.civil(2007,2,27) >> -2).should == Date.civil(2006, 12, 27)
  end

  it "should result in the last day of a month if the day doesn't exist" do
    d = Date.civil(2008,3,31) >> 1
    d.should == Date.civil(2008, 4, 30)
  end

  it "should keep the same class as the receiver" do
    c = Class.new(Date)
    c.jd.>>(-10).should be_kind_of(c)
  end

  it "should raise an error on non numeric parameters" do
    lambda { Date.civil(2007,2,27) >> "hello" }.should raise_error(TypeError)
    lambda { Date.civil(2007,2,27) >> Date.new }.should raise_error(TypeError)
    lambda { Date.civil(2007,2,27) >> Object.new }.should raise_error(TypeError)
  end
  
end
