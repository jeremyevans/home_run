require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime.now" do
  it "should be right now as a DateTime" do
    t = Time.now
    d = DateTime.now
    d.year.should == t.year
    d.mon.should == t.mon
    d.day.should == t.day
    d.hour.should == t.hour
    d.min.should == t.min
    d.sec.should == t.sec
  end
  
  it "should accept an optional sg value" do
    DateTime.now(1).to_s.should == DateTime.now.to_s
  end

  it "should keep the same class as the receiver" do
    c = Class.new(DateTime)
    c.now.should be_kind_of(c)
  end
end
