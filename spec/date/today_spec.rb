require File.expand_path('../../spec_helper', __FILE__)

describe "Date.today" do
  it "should be today's date" do
    t = Time.now
    d = Date.today
    d.year.should == t.year
    d.mon.should == t.mon
    d.day.should == t.day
  end
  
  it ".today should have an optional sg value" do
    Date.today(1).should == Date.today
  end

  it "should keep the same class as the receiver" do
    c = Class.new(Date)
    c.today.should be_kind_of(c)
  end
end
