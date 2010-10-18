require File.expand_path('../../spec_helper', __FILE__)

describe "Date#succ" do
  it "should be the next day" do
    ds    = Date.civil(2008, 10, 11)
    ds.succ.should == Date.civil(2008, 10, 12)
    ds    = Date.civil(2008, 10, 31)
    ds.succ.should == Date.civil(2008, 11, 1)
    ds    = Date.commercial(2008, 2, 7)
    ds.succ.should == Date.commercial(2008, 3, 1)
    ds    = Date.jd(2008)
    ds.succ.should == Date.jd(2009)
    ds    = Date.ordinal(2008, 366)
    ds.succ.should == Date.ordinal(2009, 1)
  end

  it "should keep the same class as the receiver" do
    c = Class.new(Date)
    c.jd.succ.should be_kind_of(c)
  end

  it "should be aliased as next" do
    Date.civil(2008, 10, 11).next.should == Date.civil(2008, 10, 12)
  end
end
