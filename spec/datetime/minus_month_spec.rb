require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#<<" do
  it "should substract a number of months from a date" do
    (DateTime.civil(2007, 12, 27) << 10).should == DateTime.civil(2007,2,27)
    (DateTime.commercial(2007, 45, 5) << 10).should == DateTime.commercial(2007,2,2)
    (DateTime.jd(2455086) << 10).should == DateTime.jd(2454782)
    (DateTime.ordinal(2008, 315) << 10).should == DateTime.ordinal(2008, 10)
    (DateTime.civil(2007, 12, 27) << 12).should == DateTime.civil(2006,12,27)
    (DateTime.civil(2007, 12, 27) << -12).should == DateTime.civil(2008,12,27)
  end

  it "should result in the last day of a month if the day doesn't exist" do
    d = DateTime.civil(2008,3,31) << 1
    d.should == DateTime.civil(2008, 2, 29)
  end

  it "should keep the same class as the receiver" do
    c = Class.new(DateTime)
    c.jd.<<(10).should be_kind_of(c)
  end

  it "should raise an error on non numeric parameters" do
    lambda { DateTime.civil(2007,2,27) << "hello" }.should raise_error(TypeError)
    lambda { DateTime.civil(2007,2,27) << DateTime.new }.should raise_error(TypeError)
    lambda { DateTime.civil(2007,2,27) << Object.new }.should raise_error(TypeError)
  end
end
