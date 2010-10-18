require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#>>" do

  it "should add a number of months to a DateTime" do
    (DateTime.civil(2007,2,27) >> 10).should == DateTime.civil(2007, 12, 27)
    (DateTime.civil(2007,2,27) >> 10).should == DateTime.civil(2007, 12, 27)
    (DateTime.commercial(2007,2,2) >> 10).should == DateTime.commercial(2007, 45, 5)
    (DateTime.jd(2454782) >> 10).should == DateTime.jd(2455086)
    (DateTime.ordinal(2008, 10) >> 10).should == DateTime.ordinal(2008, 315)
    (DateTime.civil(2007,2,27) >> 22).should == DateTime.civil(2008, 12, 27)
    (DateTime.civil(2007,2,27) >> -2).should == DateTime.civil(2006, 12, 27)
  end

  it "should result in the last day of a month if the day doesn't exist" do
    d = DateTime.civil(2008,3,31) >> 1
    d.should == DateTime.civil(2008, 4, 30)
  end

  it "should keep the same class as the receiver" do
    c = Class.new(DateTime)
    c.jd.>>(10).should be_kind_of(c)
  end

  it "should raise an error on non numeric parameters" do
    lambda { DateTime.civil(2007,2,27) >> "hello" }.should raise_error(TypeError)
    lambda { DateTime.civil(2007,2,27) >> DateTime.new }.should raise_error(TypeError)
    lambda { DateTime.civil(2007,2,27) >> Object.new }.should raise_error(TypeError)
  end
  
end
